window.onload = function() {
  cache = new GistCache();
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
  // add since parameter if cache already contains gists
  if (cache.mostRecent !== undefined) {
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
      if (!this.known.hasOwnProperty(g.id)) {
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
  var inputs = document.getElementById('languages').getElementsByName('input');
  inputs.forEach(function(el) {
    if (el.checked)
      selections[el.name] = true;
  });
  
  // only apply filter if at least one checkbox is selected
  if (Object.keys(selections).length) {
    gists.forEach(function(g) {
      if (selections.hasOwnProperty(g.files[0].language))
        results.push(g);
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
      if (results !== null) {
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
        }
      } else {
        alert('Failed to retrieve gists from GitHub.');
      }
    }
  };
  req.open('GET', params.getUrl());
  req.send();
}
