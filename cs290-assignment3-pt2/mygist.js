window.onload = function() {
  cache = new GistCache();
}

/**
* Creates valid request parameters based on the HTML elements.
*/
function RequestParams() {
  this.url = 'https://api.github.com/gists';
  this.count = Number.parseInt(document.getElementsByName('count')[0].value);
  // validate count argument and set to 30 if less than 30
  if (typeof this.count !== 'number' || this.count < 30) {
    this.count = 30;
  }
  this.currentPage = 1;
  this.totalPages = function() {
    return Math.ceil(this.count / 100);
  };
  
  this.getUrl = function() {
    var newUrl = this.url + '?page=';
    if (this.totalPages() === 1) {
      newUrl = newUrl + '1&per_page=' + this.count;
    } else {
      newUrl = newUrl + this.currentPage + '&per_page=100';
    }
    // add since parameter if cache already contains gists
    if (cache.mostRecent !== undefined) {
      newUrl = newUrl + '&since=' + cache.mostRecent;
    }
    return newUrl;
  };
}

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
    this.saveCache();
  } else {
    throw 'Operation failed: Argument is not an array of gists.';
  }
}

/**
* Compares two Gist objects in an array based on update date.
* Logic taken from http://stackoverflow.com/questions/1129216/sorting-objects-in-an-array-by-a-field-value-in-javascript
*/
function compareGists(a,b) {
  if (a.updated_at < b.updated_at)
    return -1;
  if (a.updated_at > b.updated_at)
    return 1;
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

function runQuery() {
  var params = new RequestParams();
  
  // continue to requests gists until required number is obtained
  while (params.currentPage <= params.totalPages()) {
    getGists(params);
    params.currentPage++;
  }
  
  // sort the results and update mostRecent to reflect new data
  cache.sortGists();
  if (cache.gists.length > 0)
    cache.mostRecent = cache.gists[0].updated_at;
}

/**
* Gets gists from GitHub based on the number of gists specified in the HTML.
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
      if (results !== null)
        cache.addGists(results);
      else
        alert('Failed to retrieve gists from GitHub.');
    }
  };
  req.open('GET', params.getUrl());
  req.send();
}