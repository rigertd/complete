/**
 * @file This file contains the functions and symbols required to display charts
 *       for the Child Growth Tracker website.
 * @author David Rigert [rigertd@onid.oregonstate.edu]
 * @copyright 2015 David Rigert
 */

if (typeof GrowthTracker == 'undefined') {
  var GrowthTracker = { };
}
if (typeof GrowthTracker.Chart == 'undefined') {
  /**
   * The <code>Chart</code> namespace contains the functions specific to
   * chart creation and display.
   * @namespace
   */
  GrowthTracker.Chart = { };
}

/* load google visualization library */
google.load('visualization', '1', {packages: ['corechart']});
google.setOnLoadCallback(function() {GrowthTracker.Chart.loaded = true;});

/**
 * Sets the event handlers when the page is finished loading.
 */
window.onload = function() {
  /* page elements to set event handlers for */
  var newButton = document.getElementById('newButton');
  var saveButton = document.getElementById('addButton');
  var cancelButton = document.getElementById('cancelButton');
  var deleteButton = document.getElementById('deleteButton');
  var addDataButton = document.getElementById('addDataButton');
  var saveDataButton = document.getElementById('saveDataButton');
  var cancelDataButton = document.getElementById('cancelDataButton');
  var newProfileForm = document.getElementById('newProfileForm');
  var addDataForm = document.getElementById('addDataForm');
  var profileSelector = document.getElementById('profile');
  var chartTypeSelector = document.getElementById('chartType');
  var chartDiv = document.getElementById('chartDiv');
  var profileName = document.getElementById('profileName');
  var profileDob = document.getElementById('profileDob');
  var profileGender = document.getElementById('profileGender');
  var welcome = document.getElementById('welcome');

  /* Hide new profile form and add data form by default */
  newProfileForm.style.display = 'none';
  addDataForm.style.display = 'none';

  addDataButton.onclick = function() {
    if (GrowthTracker.Chart.validateProfile()) {
      GrowthTracker.Chart.setUnitSelector();
      GrowthTracker.Chart.showForm(addDataForm, addDataButton);
      GrowthTracker.Chart.hideForm(newProfileForm, newButton);
    }
    else return false;
  };

  cancelDataButton.onclick = function() {
    GrowthTracker.Chart.hideForm(addDataForm, addDataButton);
  };

  saveDataButton.onclick = function() {
    if (GrowthTracker.Chart.validateProfile()) {
      GrowthTracker.Chart.addCheckupData();
    }
    else return false;
  };

  /* validate profile select elements */
  profileSelector.onchange = function() {
    if (GrowthTracker.validateSelect(this, 'You must select a profile.')) {
      if (chartTypeSelector.value) {
        welcome.style.display = 'none';
        GrowthTracker.Chart.loadChart();
      } else if (GrowthTracker.Chart.chart) {
        GrowthTracker.Chart.chart.clearChart();
        welcome.style.display = '';
      }
    } else if (GrowthTracker.Chart.chart) {
      GrowthTracker.Chart.chart.clearChart();
      welcome.style.display = '';
    }
  };
  chartTypeSelector.onchange = function() {
    if (GrowthTracker.validateSelect(this, 'You must select a chart type.')) {
      GrowthTracker.Chart.setUnitSelector();
      if (profileSelector.value) {
        welcome.style.display = 'none';
        GrowthTracker.Chart.loadChart();
      } else if (GrowthTracker.Chart.chart) {
        GrowthTracker.Chart.chart.clearChart();
        welcome.style.display = '';
      }
    } else if (GrowthTracker.Chart.chart) {
      GrowthTracker.Chart.chart.clearChart();
      welcome.style.display = '';
    }
  };

  /* display form if Add New Profile button is clicked */
  newButton.onclick = function() {
    GrowthTracker.Chart.hideForm(addDataForm, addDataButton);
    GrowthTracker.Chart.showForm(newProfileForm, newButton);
  };

  /* If user clicks Save, commit data to database */
  saveButton.onclick = GrowthTracker.Chart.addNewProfile;

  /* validate new profile input elements */
  profileName.onblur = GrowthTracker.validateElement;
  profileDob.onblur = GrowthTracker.validateElement;
  profileGender.onchange = function() {
    GrowthTracker.validateSelect(this, 'You must select a gender.')
  };

  /* If user clicks cancel, hide new profile form and remove any error messages */
  cancelButton.onclick = function() {
    GrowthTracker.Chart.hideForm(newProfileForm, newButton);
  };

  /* delete selected profile */
  deleteButton.onclick = function() {
    if (GrowthTracker.Chart.removeProfile()) {
      GrowthTracker.Chart.hideForm(newProfileForm, newButton);
    }
    return false;
  };
};

/**
 * Hides the specified button and displays the specified form.
 * @param {Node} form   - The form to display.
 * @param {Node} button - The button to hide.
 */
GrowthTracker.Chart.showForm = function(form, button) {
  form.style.display = '';
  button.style.display = 'none';
};

/**
 * Hides and clears the specified form and makes the specified button visible.
 * @param {Node} form   - The form to hide and reset.
 * @param {Node} button - The button to display.
 */
GrowthTracker.Chart.hideForm = function(form, button) {
  form.style.display = 'none';
  button.style.display = '';
  GrowthTracker.clearForm(form);
};

/**
 * Sets the specified selector to imperial and metric length units.
 * @param {Node} selector - The select node to add the options to.
 */
GrowthTracker.Chart.setLengthUnits = function(selector) {
  while (selector.hasChildNodes()) {
    selector.removeChild(selector.lastChild);
  }
  var op = new Option('value', 'in');
  op.textContent = 'Inches';
  selector.appendChild(op);
  op = new Option('value', 'cm');
  op.textContent = 'Centimeters';
  selector.appendChild(op);

};

/**
 * Sets the specified selector to imperial and metric weight units.
 * @param {Node} selector - The select node to add the options to.
 */
GrowthTracker.Chart.setWeightUnits = function(selector) {
  while (selector.hasChildNodes()) {
    selector.removeChild(selector.lastChild);
  }
  var op = new Option('value', 'lb');
  op.textContent = 'Pounds';
  selector.appendChild(op);
  op = new Option('value', 'kg');
  op.textContent = 'Kilograms';
  selector.appendChild(op);
};

/**
 * Sets the unit selectors in the add data form to the previous values
 * and sets event handlers to save any changes to a cookie.
 */
GrowthTracker.Chart.setUnitSelector = function() {
  var unit = document.getElementById('dataUnit');
  var entry = document.getElementById('dataEntry');
  var chartType = document.getElementById('chartType');

  switch (chartType.options[chartType.selectedIndex].value) {
    case 'length':
      GrowthTracker.Chart.setLengthUnits(unit);
      entry.placeholder = 'Body Length';
      unit.value = docCookies.getItem('lengthUnit') || 'in';
      unit.onchange = function() {
        docCookies.setItem('lengthUnit', unit.value, Infinity);
      };
      break;
    case 'weight':
      GrowthTracker.Chart.setWeightUnits(unit);
      entry.placeholder = 'Weight';
      unit.value = docCookies.getItem('weightUnit') || 'lb';
      unit.onchange = function() {
        docCookies.setItem('weightUnit', unit.value, Infinity);
      };
      break;
    case 'head':
      GrowthTracker.Chart.setLengthUnits(unit);
      entry.placeholder = 'Head Circumference';
      unit.value = docCookies.getItem('headUnit') || 'in';
      unit.onchange = function() {
        docCookies.setItem('headUnit', unit.value, Infinity);
      };
      break;
  }
};

/**
 * Resizes the chart when the browser window is resized.
 */
window.onresize = function() {
  if (GrowthTracker.Chart.options) {
    GrowthTracker.Chart.options.width = GrowthTracker.Chart.calculateWidth();
    GrowthTracker.Chart.options.height = GrowthTracker.Chart.options.width * 0.8;
    GrowthTracker.Chart.options.chartArea.width = GrowthTracker.Chart.options.width * 0.9 - 80;
    GrowthTracker.Chart.chart.draw(GrowthTracker.Chart.view, GrowthTracker.Chart.options);
  }
};

/**
 * Loads the chart based on the selected profile and chart type.
 * Hides the add new profile form.
 */
GrowthTracker.Chart.loadChart = function() {
  var newProfileForm = document.getElementById('newProfileForm');
  var newButton = document.getElementById('newButton');
  newProfileForm.style.display = 'none';
  newButton.style.display = '';
  GrowthTracker.clearForm(newProfileForm);
  GrowthTracker.Chart.getPercentileData();
};

/**
 * Removes the currently selected profile from the database.
 * Hides the add new profile form.
 * @returns {boolean} Whether form validation succeeded.
 */
GrowthTracker.Chart.removeProfile = function() {
  var profile = document.getElementById('profile');
  var pid = profile.options[profile.selectedIndex].value;
  if (!GrowthTracker.validateSelect(profile, 'You must select a profile.')) {
    return false;
  }
  /* verify user wants to delete */
  if (!confirm('Are you sure you want to remove the profile ' +
               profile.options[profile.selectedIndex].text +
               '? This operation cannot be undone.')) {
    return false;
  }
  /* XMLHttpRequest params */
  var url = 'chart.php?action=delete&profile=' + pid;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);
      if (result['success'] == 'true') {
        var newProfileForm = document.getElementById('newProfileForm');
        var newButton = document.getElementById('newButton');
        newProfileForm.style.display = 'none';
        newButton.style.display = '';
        GrowthTracker.clearForm(newProfileForm);
        GrowthTracker.Chart.refreshProfiles();
      } else {
        alert(result['message']);
      }
    }
  };
  xhr.open('GET', url);
  xhr.send();
  return true;
};

/**
 * Adds height, weight, and head circumference data
 * to the currently selected profile.
 */
GrowthTracker.Chart.addCheckupData = function() {
  var addDataForm = document.getElementById('addDataForm');
  var addDataButton = document.getElementById('addDataButton');
  var pid = document.getElementById('profile').value;
  var dataType = document.getElementById('chartType').value;
  var months = document.getElementById('dataMonths');
  var data = document.getElementById('dataEntry');
  var unit = document.getElementById('dataUnit');

  /* Validate user input */
  if (!GrowthTracker.Chart.validateDataForm()) {
    return false;
  }

  /* XMLHttpRequest params */
  var url = 'chart.php?action=data&profile=' + pid + '&mo=' + months.value;
  switch (dataType) {
    case 'length':
      url += '&l=' + data.value + '&lu=' + unit.value;
      break;
    case 'weight':
      url += '&w=' + data.value + '&wu=' + unit.value;
      break;
    case 'head':
      url += '&hc=' + data.value + '&hcu=' + unit.value;
      break;
  }
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);

      if (result['success'] == 'false') {
        alert(result['message']);
        return false;
      }

      GrowthTracker.Chart.hideForm(addDataForm, addDataButton);
      /* refresh chart data if chartType is selected */
      if (dataType) GrowthTracker.Chart.getPercentileData();
    }
  };
  xhr.open('GET', url);
  xhr.send();
};

/**
 * Validates the addDataForm form.
 * @returns {boolean} Whether the required fields are complete.
 */
GrowthTracker.Chart.validateDataForm = function() {
  var months = document.getElementById('dataMonths');
  var data = document.getElementById('dataEntry');

  var valid = GrowthTracker.Chart.validateProfile();
  valid = GrowthTracker.validateElement(months) && valid;
  valid = GrowthTracker.validateElement(data) && valid;
  return valid;
};

/**
 * Refreshes the entries in the Select Profile select element.
 * Hides chart and add new data form.
 */
GrowthTracker.Chart.refreshProfiles = function () {
  var profile = document.getElementById('profile');
  var addDataForm = document.getElementById('addDataForm');
  var welcome = document.getElementById('welcome');

  /* XMLHttpRequest params */
  var url = 'chart.php?action=profiles';
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);

      /* remove all options except default placeholder */
      while (profile.options.length > 1) {
        profile.removeChild(profile.lastChild);
      }
      result.forEach(function(p) {
        var op = new Option('value', p['id']);
        op.textContent = p['name'] + ' (' + p['dob'] + ')';
        profile.appendChild(op);
      });
      if (GrowthTracker.Chart.chart) GrowthTracker.Chart.chart.clearChart();
      addDataForm.style.display = 'none';
      GrowthTracker.clearForm(addDataForm);
      welcome.style.display = '';
    }
  };
  xhr.open('GET', url);
  xhr.send();
};

/**
 * Adds a new profile based on the data in the createNew form.
 * @returns {boolean} Whether form validation succeeded.
 */
GrowthTracker.Chart.addNewProfile = function() {
  var name = document.getElementById('profileName');
  var dob = document.getElementById('profileDob');
  var gender = document.getElementById('profileGender');
  var newForm = document.getElementById('newProfileForm');
  var newButton = document.getElementById('newButton');

  /* Validate user input */
  var valid = GrowthTracker.validateElement(name);
  valid = GrowthTracker.validateElement(dob) && valid;
  valid = GrowthTracker.validateSelect(gender, 'You must select a gender.') && valid;
  if (!valid) {
    return false;
  }

  /* XMLHttpRequest params */
  var url = 'chart.php?action=new&profileName=' + encodeURIComponent(name.value) +
            '&profileDob=' + encodeURIComponent(dob.value) +
            '&profileGender=' + gender.value;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);
      if (result['success'] == 'true') {
        GrowthTracker.Chart.refreshProfiles();
        /* Hide form and show new button again */
        GrowthTracker.toggleVisible(newForm);
        GrowthTracker.toggleVisible(newButton);
      } else {
        alert(result['message']);
      }
    }
  };
  xhr.open('GET', url);
  xhr.send();
  return true;
};

/**
 * Converts the specified javascript array from the server into a DataTable.
 * @param {Array} arr - An array of rows returned from the database.
 * @returns {google.visualization.DataTable} The DataTable to load in the line chart.
 */
GrowthTracker.Chart.prepareData = function(arr) {
  var i, j;
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Age in Months');
  data.addColumn('number', '99.9th');
  data.addColumn('number', '99th');
  data.addColumn('number', '97th');
  data.addColumn('number', '95th');
  data.addColumn('number', '90th');
  data.addColumn('number', '85th');
  data.addColumn('number', '75th');
  data.addColumn('number', '50th');
  data.addColumn('number', '25th');
  data.addColumn('number', '15th');
  data.addColumn('number', '10th');
  data.addColumn('number', '5th');
  data.addColumn('number', '3rd');
  data.addColumn('number', '1st');
  data.addColumn('number', '0.1th');
  data.addColumn('number', 'Actual');
  for (i = 0; i < arr.length; ++i) {
    arr[i][0] = String(arr[i][0]);
    for (j = 1; j < arr[i].length; ++j) {
      arr[i][j] = parseFloat(arr[i][j]) || null;
    }
  }
  data.addRows(arr);
  return data;
};

/**
 * Validates the profile and chart type select elements.
 * This is used to determine whether the placeholders are selected.
 * @returns {boolean} Whether both values are non-empty.
 */
GrowthTracker.Chart.validateProfile = function() {
  var selector = document.getElementById('chartType');
  var profile = document.getElementById('profile');

  GrowthTracker.validateSelect(selector, 'You must select a chart type.');
  GrowthTracker.validateSelect(profile, 'You must select a profile.');

  return selector.options[selector.selectedIndex].value
      && profile.options[profile.selectedIndex].value;
};

/**
 * Gets the percentile data for the selected chart type from the database.
 */
GrowthTracker.Chart.getPercentileData = function() {
  /* get drop-down values */
  var selector = document.getElementById('chartType');
  var chartType = selector.options[selector.selectedIndex].value;
  var chartName = selector.options[selector.selectedIndex].text;
  var profile = document.getElementById('profile');
  var pid = profile.options[profile.selectedIndex].value;

  /* validate input */
  if (!GrowthTracker.Chart.validateProfile()) return false;

  /* XMLHttpRequest params */
  var url = 'chart.php?action=chart&profile=' + pid + '&type=' + chartType;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);
      GrowthTracker.Chart.data = GrowthTracker.Chart.prepareData(result);
      /* stop if there was a problem loading google library */
      if (!GrowthTracker.Chart.loaded) return;
      /* build the chart */
      GrowthTracker.Chart.buildChart(chartType, chartName);
    }
  };

  xhr.open('GET', url);
  xhr.send();
};

/**
 * Builds the actual chart for the specified type
 * based on the DataTable set in GrowthTracker.Chart.data.
 * @param {string} chartType - The chart type ('length', 'weight', or 'head').
 * @param {string} chartName - The name of the selected chart type.
 */
GrowthTracker.Chart.buildChart = function(chartType, chartName) {
  /* determine size */
  var chartDiv = document.getElementById('chartDiv');
  var width = GrowthTracker.Chart.calculateWidth();
  var unit = chartType == 'weight' ? 'Kilograms' : 'Centimeters';

  GrowthTracker.Chart.view = new google.visualization.DataView(GrowthTracker.Chart.data);
  GrowthTracker.Chart.view.hideColumns([1,4,6,10,12,15]);
  GrowthTracker.Chart.options = {
    title: chartName + ' Percentiles',
    curveType: 'function',
    width: width,
    height: width * 0.8,
    chartArea: {
      left:50,top:25,width:width * 0.9 - 80,height:'80%'
    },
    interpolateNulls: true,
    series: {
      0: {targetAxisIndex: 0, lineWidth: 1},
      1: {targetAxisIndex: 0, lineWidth: 1},
      2: {targetAxisIndex: 0, lineWidth: 1},
      3: {targetAxisIndex: 0, lineWidth: 1},
      4: {targetAxisIndex: 0, lineWidth: 1},
      5: {targetAxisIndex: 0, lineWidth: 1},
      6: {targetAxisIndex: 0, lineWidth: 1},
      7: {targetAxisIndex: 0, lineWidth: 1},
      8: {targetAxisIndex: 0, lineWidth: 1},
      9: {targetAxisIndex: 0, pointShape: 'circle', pointSize: 5, color: 'black'}
    },
    hAxis: {
      showTextEvery: 3
    },
    hAxes: {
      0: {title: 'Age in Months'}
    },
    vAxes: {
      0: {title: chartName + ' in ' + unit}
    }
  };
  GrowthTracker.Chart.chart = new google.visualization.LineChart(chartDiv);
  GrowthTracker.Chart.chart.draw(GrowthTracker.Chart.view, GrowthTracker.Chart.options);
};

/**
 * Calculates the width to use for the chart.
 * @returns {number} Width to use for the chart.
 */
GrowthTracker.Chart.calculateWidth = function() {
  var chartDiv = document.getElementById('chartDiv');
  return chartDiv.parentNode.clientWidth * 0.99;
};
