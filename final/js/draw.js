/**
 * @file This file contains the functions and symbols required to display charts
 *       for the Child Growth Tracker website.
 * @author David Rigert [rigertd@onid.oregonstate.edu]
 * @copyright 2015 David Rigert
 */

if (typeof GrowthTracker == 'undefined') {
  /**
   * The <code>GrowthTracker</code> namespace contains all of the symbols
   * and functions required for the Baby Growth Tracker website.
   * @namespace
   */
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

google.load('visualization', '1', {packages: ['corechart']});
google.setOnLoadCallback(function() {GrowthTracker.Chart.loaded = true;});

window.onload = function() {
  var profileBtn = document.getElementById('profileButton');
  profileBtn.onclick = GrowthTracker.Chart.getPercentileData;
  var profileSelector = document.getElementById('profile');
  profileSelector.onchange = function() {
    GrowthTracker.validateSelect(this, 'You must select a profile.');
  };
  var chartTypeSelector = document.getElementById('chartType');
  chartTypeSelector.onchange = function() {
    GrowthTracker.validateSelect(this, 'You must select a chart type.');
  };

  var newForm = document.getElementById('createNew');
  newForm.style.display = 'none';
  var newButton = document.getElementById('newButton');
  newButton.onclick = function() {
    GrowthTracker.toggleVisible(newForm);
    GrowthTracker.toggleVisible(newButton);
  };
  var saveButton = document.getElementById('addButton');
  saveButton.onclick = GrowthTracker.Chart.addNewProfile;

  var profileName = document.getElementById('profileName');
  var profileDob = document.getElementById('profileDob');
  var profileGender = document.getElementById('profileGender');
  profileName.onblur = GrowthTracker.validateElement;
  profileDob.onblur = GrowthTracker.validateElement;
  profileGender.onchange = function() {
    GrowthTracker.validateSelect(this, 'You must select a gender.')
  };

  var cancelButton = document.getElementById('cancelButton');
  cancelButton.onclick = function() {
    GrowthTracker.hideError(profileName);
    GrowthTracker.removeValidationMessage(profileName.id + '_msg');
    GrowthTracker.hideError(profileDob);
    GrowthTracker.removeValidationMessage(profileDob.id + '_msg');
    GrowthTracker.hideError(profileGender);
    GrowthTracker.removeValidationMessage(profileGender.id + '_msg');
    GrowthTracker.toggleVisible(newForm);
    GrowthTracker.toggleVisible(newButton);
  }
  var deleteButton = document.getElementById('deleteButton');
  deleteButton.onclick = GrowthTracker.Chart.removeProfile;
};

window.onresize = function() {
  if (GrowthTracker.Chart.options) {
    GrowthTracker.Chart.options.width = GrowthTracker.Chart.calculateWidth();
    GrowthTracker.Chart.options.height = GrowthTracker.Chart.options.width * 0.8;
    GrowthTracker.Chart.options.chartArea.width = GrowthTracker.Chart.options.width * 0.9 - 80;
    GrowthTracker.Chart.chart.draw(GrowthTracker.Chart.view, GrowthTracker.Chart.options);
  }
};

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

      alert(result.message);
      GrowthTracker.Chart.refreshProfiles();
    }
  };

  xhr.open('GET', url);
  xhr.send();
};

GrowthTracker.Chart.refreshProfiles = function () {
  var profile = document.getElementById('profile');

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
        var op = new Option('value', p.id);
        op.textContent = p.name + ' (' + p.dob + ')';
        profile.appendChild(op);
      });
    }
  };

  xhr.open('GET', url);
  xhr.send();
};

GrowthTracker.Chart.addNewProfile = function() {
  var name = document.getElementById('profileName');
  var dob = document.getElementById('profileDob');
  var gender = document.getElementById('profileGender');
  var newForm = document.getElementById('createNew');
  var newButton = document.getElementById('newButton');

  var valid = GrowthTracker.validateElement(name);
  valid = GrowthTracker.validateElement(dob) && valid;
  valid = GrowthTracker.validateSelect(gender, 'You must select a gender.') && valid;

  if (!valid) {
    return false;
  }

  GrowthTracker.toggleVisible(newForm);
  GrowthTracker.toggleVisible(newButton);
};

GrowthTracker.Chart.prepareData = function(arr) {
  var i, j;
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Age in Months');
  data.addColumn('number', '99.9th')
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
  data.addColumn('number', 'Actual')
  for (i = 0; i < arr.length; ++i) {
    arr[i][0] = String(arr[i][0]);
    for (j = 1; j < arr[i].length; ++j) {
      arr[i][j] = parseFloat(arr[i][j]) || null;
    }
  }
  data.addRows(arr);
  return data;
};

GrowthTracker.Chart.validateProfile = function(pid, chartType) {
  var selector = document.getElementById('chartType');
  var profile = document.getElementById('profile');

  GrowthTracker.validateSelect(selector, 'You must select a chart type.');
  GrowthTracker.validateSelect(profile, 'You must select a profile.');

  return pid && chartType;
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
  if (!GrowthTracker.Chart.validateProfile(pid, chartType)) return false;

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

GrowthTracker.Chart.calculateWidth = function() {
  var chartDiv = document.getElementById('chartDiv');
  return chartDiv.parentNode.clientWidth * 0.99;
};