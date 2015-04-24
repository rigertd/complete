window.onload = function() {
  cache = new GistCache();
  favList = new FavoritesList();
  createFavoritesList(favList.favorites);
};

/**
* This object is used to create a valid request URL based on the settings
* specified in the HTML document.
*/
function RequestParams() {
  this.url = 'https://api.github.com/gists';
  this.count = Number.parseInt(document.getElementsByName('count')[0].value);
  this.currentPage = 1;
  
  // validate count value and set to 30 if less than 30
  if (typeof this.count !== 'number' || this.count < 30) {
    this.count = 30;
  }
}

/**
* Advances to the next page and returns true if one is available.
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
* Gets the total number of pages required to download the specified count
*/
RequestParams.prototype.totalPages = function() {
  return Math.ceil(this.count / 100);
};

/**
* Gets the number of gists to download per page.
*/
RequestParams.prototype.perRequest = function() {
  if (this.count <= 100) {
      return this.count;
  } else {
      return Math.ceil(this.count/this.totalPages());
  }
};

/**
* Gets the URL string for the current page to download.
*/
RequestParams.prototype.getUrl = function() {
  var newUrl = this.url + '?page=';
  if (this.totalPages() === 1) {
    newUrl = newUrl + '1&per_page=' + this.count;
  } else {
    newUrl = newUrl + this.currentPage + '&per_page=' + this.perRequest();
  }
  // add since parameter if cache already contains gists unless count was increased
  if (cache.mostRecent !== undefined && cache.gists.length >= this.count) {
    newUrl = newUrl + '&since=' + cache.mostRecent;
  }
  return newUrl;
};

/**
* This object maintains a list of downloaded gists 
* and stores them in sessionStorage to reduce network usage.
*/
function GistCache() {
  // get list of gists from sessionStorage cache if available
  this.gists = JSON.parse(sessionStorage.getItem('gistCache'));
  this.known = JSON.parse(sessionStorage.getItem('knownGists'));
  if (this.gists === null) {
    this.gists = [];
    sessionStorage.setItem('gistCache', JSON.stringify(this.gists));
  }
  if (this.known === null) {
    this.known = {};
    sessionStorage.setItem('knownGists', JSON.stringify(this.known));
  }
  
  if (this.gists.length > 0)
    this.mostRecent = this.gists[0].updated_at;
}

/**
* Finds and returns a Gist in the cache by the Gist ID.
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
*/
GistCache.prototype.trimGists = function(count) {
  var removed = this.gists.splice(count, this.gists.length - count);
  removed.forEach(function(g) {
    this.known[g.id] = undefined;
  }, this);
};

/**
* Saves the gist cache settings to sessionStorage.
*/
GistCache.prototype.saveCache = function() {
  sessionStorage.setItem('gistCache', JSON.stringify(this.gists));
  sessionStorage.setItem('knownGists', JSON.stringify(this.known));
}

/**
* Sorts the gist array in the cache based on the update date.
*/
GistCache.prototype.sortGists = function() {
  this.gists.sort(compareGists);
}

/**
* Adds an array of Gists to the GistCache object.
* Only adds new and updated Gists.
* @param {object[]} gistArray   An array of Gist objects.
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
* Compares two Gist objects in an array based on update date. Sorts from newest to oldest
* Logic taken from http://stackoverflow.com/questions/1129216/sorting-objects-in-an-array-by-a-field-value-in-javascript
*/
function compareGists(a,b) {
  if (a.updated_at < b.updated_at)
    return 1;
  if (a.updated_at > b.updated_at)
    return -1;
  return 0;
}

/**
* Filters the list of Gists based on the selected language filters.
* @param {Object[]} gists   An array of Gist objects.
* @return {Object[]} A filtered array of Gist objects.
*/
function filterResults(gists) {
  var selections = {};
  var results = [];
  var inputs = document.getElementById('languages').getElementsByTagName('input');
  for (var i = 0; i < inputs.length; i++) {
    if (inputs[i] && inputs[i].checked) {
      selections[inputs[i].name] = true;
    }
  }
  
  // only apply filter if at least one checkbox is selected
  if (Object.keys(selections).length > 0) {
    gists.forEach(function(g) {
      for (f in g.files) {
        if (selections.hasOwnProperty(g.files[f].language)) {
        results.push(g);
        return;
        }
      }
    });
    return results;
  } else {
    return gists;
  }
}

/**
* Creates the request parameters and initiates the request.
*/
function runQuery() {
  var params = new RequestParams();
  getGists(params);
}

/**
* Gets gists from GitHub based on the specified request parameters.
*/
function getGists(params) {
  // need to create multiple XMLHttpRequests for concurrent data retrieval
  // try to create XMLHttpRequest
  var req = new XMLHttpRequest();
  if (!req) {
    throw 'Cannot create HttpRequest.';
  }
  
  // add gists to cache when retrieved
  req.onreadystatechange = function() {
    if (this.readyState === 4) {
      var results = JSON.parse(this.responseText);
      if (this.status === 200 && results !== null) {
        cache.addGists(results);
        // recursively get more gists if there are more pages
        if (results.length == params.perRequest() 
          && params.nextPage()) {
          getGists(params);
        } else {
          // sort and save the cache
          cache.sortGists();
          cache.saveCache();
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
    li.appendChild(createGistEntry(g, true));
    list.appendChild(li);
    // break after displaying specified number of gists
    if (i++ >= count) {
      return true;
    }
  });
}

function createFavoritesList(favs) {
  var list = document.getElementById('favorites');
  // clear old values
  while (list.lastChild) {
    list.removeChild(list.lastChild);
  }

  if (Object.keys(favs).length > 0) {
    for (gist in favs) {
      var li = document.createElement('li');
      li.id = gist;
      li.appendChild(createGistEntry(favs[gist], false));
      list.appendChild(li);
    }
  } else {
    var li = document.createElement('li');
    li.textContent = 'You do not have any favorites';
    list.appendChild(li);
  }
}

function createGistEntry(gist, add) {
  var ul = document.createElement('ul'),
      liFav = document.createElement('li'),
      btnFav = document.createElement('button'),
      liGist = document.createElement('li'),
      aGist = document.createElement('a');
  
  if (add) {
    btnFav.title = 'Add to Favorites';
    btnFav.onclick = addToFavorites;
    btnFav.className = 'add_fav';
  } else {
    btnFav.title = 'Remove from Favorites';
    btnFav.onclick = removeFromFavorites;
    btnFav.className = 'remove_fav';
  }
  btnFav.value = gist.id;
  liFav.appendChild(btnFav);
  
  aGist.href = gist.html_url;
  aGist.textContent = (gist.description === ''
                    || gist.description === null
                     ? '(No description)'
                     : gist.description);
  liGist.appendChild(aGist);
  liGist.appendChild(createLangList(gist.files));
  
  // append add to favorites button
  ul.appendChild(liFav);
  // append Gist info
  ul.appendChild(liGist);
  
  return ul;
}

/**
* Creates an unordered list of unique languages in the files of a Gist.
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

function FavoritesList() {
  this.favorites = JSON.parse(localStorage.getItem('favoritesList'));
  if (this.favorites === null) {
    this.favorites = {};
    localStorage.setItem('favoritesList', JSON.stringify(this.favorites));
  }
}

FavoritesList.prototype.saveFavorites = function() {
  localStorage.setItem('favoritesList', JSON.stringify(this.favorites));
}

FavoritesList.prototype.removeFavorite = function(favId) {
  delete this.favorites[favId];
  this.saveFavorites();
}

function addToFavorites() {
  favList.favorites[this.value] = cache.findByGistId(this.value);
  favList.saveFavorites();
  var params = new RequestParams();
  createGistList(cache.gists, params.count);
  createFavoritesList(favList.favorites);
}

function removeFromFavorites() {
  var target = document.getElementById(this.value);
  favList.removeFavorite(this.value);
  target.parentElement.removeChild(target);
  favList.saveFavorites();
  var params = new RequestParams();
  createGistList(cache.gists, params.count);
  createFavoritesList(favList.favorites);
}

