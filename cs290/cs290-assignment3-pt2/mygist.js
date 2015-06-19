/**
* @file This implements a custom gist listing service that
*       downloads gists from the GitHub API and filters them by language.
*       The user can optionally maintain a list of favorite gists.
* @author David Rigert [rigertd@onid.oregonstate.edu]
*/

/**
* Callback to create global cache objects and write favorites to DOM.
*/
window.onload = function() {
  cache = new GistCache();
  favList = new FavoritesList();
  createFavoritesList(favList.favorites);
};

/**
* This object stores the user's favorite gists in memory and in localStorage.
* Favorites are retained as long as localStorage persists.
* This object gets the favorites list from localStorage if it already exists
* or creates a new one and saves it to localStorage if not.
* @constructor
*/
function FavoritesList() {
  this.favorites = JSON.parse(localStorage.getItem('favoritesList'));
  if (this.favorites === null) {
    this.favorites = {};
    localStorage.setItem('favoritesList', JSON.stringify(this.favorites));
  }
}

/**
* Saves the current list of favorites to localStorage.
*/
FavoritesList.prototype.saveFavorites = function() {
  localStorage.setItem('favoritesList', JSON.stringify(this.favorites));
};

/**
* Removes the favorite with the specified gist ID from memory and localStorage.
* @param {string} gistId - ID of the gist to remove.
*/
FavoritesList.prototype.removeFavorite = function(gistId) {
  delete this.favorites[gistId];
  this.saveFavorites();
};

/**
* This object maintains a list of downloaded gists
* and stores them in memory to reduce network usage.
* Known gist IDs are stored in {@link GistCache.known} for fast lookup.
* An array of Gist objects is stored in {@link GistCache.gists}.
* @constructor
*/
function GistCache() {
  this.gists = [];
  this.known = {};
}

/**
* Finds and returns a Gist in the cache by the Gist ID.
* @param {string} gistId - ID of the gist to find.
* @return {Object|undefined} The gist object if it is found in the cache.
*/
GistCache.prototype.findByGistId = function(gistId) {
  // first make sure it exists in the cache
  if (this.known.hasOwnProperty(gistId)) {
    for (var i = 0; i < this.gists.length; i++) {
      if (this.gists[i].id === gistId) {
        return this.gists[i];
      }
    }
  }
};

/**
* Removes Gists in excess of the current count from the cache.
* Sets the known gists to undefined to avoid performance hit of delete.
* @param {number} count - Number of gists to leave in the cache.
*/
GistCache.prototype.trimGists = function(count) {
  var removed = this.gists.splice(count, this.gists.length - count);
  removed.forEach(function(g) {
    this.known[g.id] = undefined;
  }, this);
};

/**
* Sorts the gist array in the cache based on the update date.
*/
GistCache.prototype.sortGists = function() {
  this.gists.sort(compareGists);
};

/**
* Adds an array of Gists to the GistCache object.
* Only adds new and updated Gists.
* @param {object[]} gistArray - An array of Gist objects.
*/
GistCache.prototype.addGists = function(gistArray) {
  if (Array.isArray(gistArray)) {
    gistArray.forEach(function(g) {
      if (this.known[g.id] === undefined) {
        // not found, add gist to cache
        this.gists.push(g);
        this.known[g.id] = g.updated_at;
      } else if (this.known[g.id] < g.updated_at) {
        // newer version of gist found; replace old one
        var i = this.gists.map(function(x) { return x.id; }).indexOf(g.id);
        this.gists[i] = g;
      }
    }, this);
  } else {
    throw 'Operation failed: Argument is not an array of gists.';
  }
};

/**
* This object is used to create a valid request URL based on the settings
* specified in the HTML document.
* @constructor
*/
function RequestParams() {
  this.url = 'https://api.github.com/gists';
  this.count = parseInt(document.getElementById('dlCount').value);
  this.currentPage = 1;

  // validate count value and set to 30 if less than 30
  if (typeof this.count !== 'number' || this.count < 30) {
    this.count = 30;
  }
}

/**
* Advances to the next page and returns true if one is available.
* @return {boolean} Returns whether another page needs to be retrieved.
*/
RequestParams.prototype.nextPage = function() {
  if (this.currentPage < this.totalPages()) {
    this.currentPage++;
    return true;
  } else {
    return false;
  }
};

/**
* Gets the total number of pages required to download the specified count.
* @return {number} Returns the total number of pages to be requested.
*/
RequestParams.prototype.totalPages = function() {
  return Math.ceil(this.count / 100);
};

/**
* Gets the number of gists to download per page.
* @return {number} Returns the number of gists to request for each page.
*/
RequestParams.prototype.perRequest = function() {
  if (this.count <= 100) {
      return this.count;
  } else {
      return Math.ceil(this.count / this.totalPages());
  }
};

/**
* Gets the URL string for the current page to download.
* @return {string} Returns the request URL for the current page.
*/
RequestParams.prototype.getUrl = function() {
  var newUrl = this.url + '?page=';
  if (this.totalPages() === 1) {
    newUrl = newUrl + '1&per_page=' + this.count;
  } else {
    newUrl = newUrl + this.currentPage + '&per_page=' + this.perRequest();
  }
  // add 'since' parameter if cache already contains gists
  // unless count was increased
  if (cache.mostRecent !== undefined && cache.gists.length >= this.count) {
    newUrl = newUrl + '&since=' + cache.mostRecent;
  }
  return newUrl;
};

/**
* This is a callback function for adding a gist to the favorites list.
* The clicked gist is removed from the results and added to the favorites list.
* @this {Button}
*/
function addToFavorites() {
  favList.favorites[this.value] = cache.findByGistId(this.value);
  favList.saveFavorites();
  removeFromResults(this.value);
  createFavoritesList(favList.favorites);
}

/**
* Compares two Gist objects in an array based on update date.
* Sorts from newest to oldest. Logic taken from:
* http://stackoverflow.com/questions/1129216/sorting-objects-in-an-array-by-a-field-value-in-javascript
* @param {Object} a - Left object to compare.
* @param {Object} b - Right object to compare.
* @return {number} Returns 1 if a is less than b,
*                  -1 if b is less than a, and 0 if a and b are equal.
*/
function compareGists(a, b) {
  if (a.updated_at < b.updated_at)
    return 1;
  if (a.updated_at > b.updated_at)
    return -1;
  return 0;
}

/**
* Writes the specified list of favorites to the DOM.
* @param {Object} favs - Object containing favorite gists.
*/
function createFavoritesList(favs) {
  var list = document.getElementById('favorites');
  // clear old values
  while (list.lastChild) {
    list.removeChild(list.lastChild);
  }

  // display message if no favorites
  if (Object.keys(favs).length == 0) {
    var li = document.createElement('li');
    li.textContent = 'You do not have any favorites';
    list.appendChild(li);
  } else {
    for (gist in favs) {
      var li = document.createElement('li');
      li.id = gist;
      li.appendChild(createGistEntry(favs[gist], false));
      list.appendChild(li);
    }
  }
}

/**
* Creates an unordered list of the components of a Gist entry.
* The components are in the following order:
*   1. 'Add to Favorites' button if {boolean} add is true,
*      'Remove from Favorites' button if false
*   2. Gist description with link to the HTML URL for that gist
*   3. Unordered list of languages included in the Gist entry
* @param {Object} gist - Single Gist object returned by GitHub API.
* @param {boolean} add - Whether the favorites button should be add or remove.
* @return {Object} UL element containing the components of a Gist entry.
*/
function createGistEntry(gist, add) {
  var ul = document.createElement('ul'),
      liFav = document.createElement('li'),
      btnFav = document.createElement('button'),
      liGist = document.createElement('li'),
      aGist = document.createElement('a');

  if (add) {  // result entry
    btnFav.title = 'Add to Favorites';
    btnFav.onclick = addToFavorites;
    btnFav.className = 'add_fav';
  } else {    // favorite list entry
    btnFav.title = 'Remove from Favorites';
    btnFav.onclick = removeFromFavorites;
    btnFav.className = 'remove_fav';
  }

  // set button value to gist ID for future reference
  btnFav.value = gist.id;
  liFav.appendChild(btnFav);
  // display '(No description)' if description is missing
  aGist.href = gist.html_url;
  aGist.textContent = (gist.description === '' ||
                       gist.description === null ?
                       '(No description)' :
                       gist.description);
  liGist.appendChild(aGist);
  liGist.appendChild(createLangList(gist.files));
  // append add to favorites button
  ul.appendChild(liFav);
  // append Gist info
  ul.appendChild(liGist);
  return ul;
}

/**
* Writes the specified list of gist results to the DOM.
* @param {Object[]} gists - Array of Gist objects from cache.
* @param {number}   count - Max number of gists to display.
*/
function createGistList(gists, count) {
  var list = document.getElementById('results');
  // clear old values
  while (list.lastChild) {
    list.removeChild(list.lastChild);
  }

  // populate unordered list based on cache
  var i = 0;
  var filtered = filterResults(cache.gists);
  filtered.some(function(g) {
    var li = document.createElement('li');
    li.id = g.id;
    li.appendChild(createGistEntry(g, true));
    list.appendChild(li);
    // break after displaying specified number of gists
    if (i++ >= count - 1) {
      return true;
    }
  });
}

/**
* Creates an unordered list of unique languages in the files of a Gist.
* @param {Object} files - Object in 'files' property of Gist JSON.
* @return {Object} UL element containing one LI element per language.
*/
function createLangList(files) {
  var ul = document.createElement('ul');
  var fileTypes = {};
  // we only want one label per file type
  for (f in files) {
    if (files[f].language) {
      fileTypes[files[f].language] = true;
    }
  }
  for (ft in fileTypes) {
    var li = document.createElement('li');
    li.textContent = ft;
    ul.appendChild(li);
  }
  return ul;
}

/**
* Filters the list of Gists based on the selected language filters.
* Also filters gists that are in the favorites list.
* @param {Object[]} gists - An array of Gist objects.
* @return {Object[]} A filtered array of Gist objects.
*/
function filterResults(gists) {
  var selections = {};
  var results = [];
  var inputs = document.getElementById('langs').getElementsByTagName('input');

  // populate map of checked languages
  for (var i = 0; i < inputs.length; i++) {
    if (inputs[i] && inputs[i].checked) {
      selections[inputs[i].name] = true;
    }
  }

  // loop through every gist
  gists.forEach(function(g) {
    // skip if in favorites
    if (favList.favorites.hasOwnProperty(g.id)) {
      return;
    }
    // only apply language filter if at least one checkbox is selected
    if (Object.keys(selections).length > 0) {
      // loop through each file
      for (f in g.files) {
        // add gist to filtered result list if language is found
        if (selections.hasOwnProperty(g.files[f].language)) {
          results.push(g);
          // skip remaining files in current gist
          return;
        }
      }
    } else {
      results.push(g);
    }
  });
  return results;
}

/**
* Gets gists from GitHub based on the specified request parameters.
* @param {RequestParams} params - Request parameters to use for the request.
*/
function getGists(params) {
  // try to create XMLHttpRequest
  var req = new XMLHttpRequest();
  if (!req) {
    throw 'Cannot create HttpRequest.';
  }

  req.onreadystatechange = function() {
    if (this.readyState === 4) {  // request complete
      var results = JSON.parse(this.responseText);
      // check if request was successful
      if (this.status === 200 && results !== null) {
        // add results to cache
        cache.addGists(results);
        // recursively get more gists if there are more pages
        if (results.length == params.perRequest() && params.nextPage()) {
          getGists(params);
        } else {
          // sort the cache
          cache.sortGists();
          // update mostRecent with new data
          if (cache.gists.length > 0)
            cache.mostRecent = cache.gists[0].updated_at;
          // write to DOM
          createGistList(cache.gists, params.count);
        }
      } else {
      alert('Unable to download Gists from GitHub.');
      }
    }
  };
  req.open('GET', params.getUrl());
  req.send();
}

/**
* This is a callback function for removing a favorite from the favorites list.
* The clicked gist is removed from memory and localStorage.
* @this {Button}
*/
function removeFromFavorites() {
  favList.removeFavorite(this.value);
  createFavoritesList(favList.favorites);
  updateResults();
}

/**
* Removes the gist with the specified ID from the list of results.
* This is used when adding a gist to the favorites list.
* The gist is only removed from the DOM, not from the GistCache object.
* @param {string} gistId - ID of the gist to remove.
*/
function removeFromResults(gistId) {
  var target = document.getElementById(gistId);
  target.parentElement.removeChild(target);
}

/**
* Creates the request parameters and initiates the request.
*/
function runQuery() {
  var params = new RequestParams();
  getGists(params);
}

/**
* This is a callback function for refreshing the list of Gist results
* based on the latest filter settings and favorites.
*/
function updateResults() {
  var params = new RequestParams();
  createGistList(cache.gists, params.count);
}

