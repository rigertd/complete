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

window.onload = function() {
  var profileBtn = document.getElementById('profileButton');
  profileBtn.onclick = GrowthTracker.Chart.getPercentileData;
  var canvas = document.getElementById('chart');
  canvas.width = canvas.parentNode.clientWidth * .8;
  canvas.height = canvas.width * .8;
};

/**
 * The options for the current chart.
 * @type {{bezierCurveTension: number, datasetFill: boolean, pointDot: boolean, pointHitDetectionRadius: number, showTooltips: boolean, responsive: boolean}}
 */
GrowthTracker.Chart.options = {
  bezierCurveTension: 0,
  datasetFill: false,
  datasetStrokeWidth: 1,
  pointDot: false,
  pointHitDetectionRadius: 5,
  scaleShowLabels: true,
  showTooltips: true,
  responsive: true,
  multiTooltipTemplate: "<%= datasetLabel %> - <%= value %>"
};

/**
 * The data object for the current chart.
 * @type {{labels: string[], datasets: Array}}
 */
GrowthTracker.Chart.data = {
  labels: ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24'],
  datasets: []
};

/**
 *
 * @param label
 * @param color
 * @param data
 * @returns {{label: *, strokeColor: *, data: *}}
 */
GrowthTracker.Chart.createDataset = function(label, color, data) {
  return {
    label: label,
    strokeColor: color,
    data: data
  };
};

/**
 *
 * @param rawData
 * @returns {Array}
 */
GrowthTracker.Chart.createDatasets = function(rawData) {
  var result = [];
  result.push(GrowthTracker.Chart.createDataset('3rd', '#f88', rawData['3']));
  result.push(GrowthTracker.Chart.createDataset('15th', '#fb8', rawData['15']));
  result.push(GrowthTracker.Chart.createDataset('50th', '#8f8', rawData['50']));
  result.push(GrowthTracker.Chart.createDataset('85th', '#8bf', rawData['85']));
  result.push(GrowthTracker.Chart.createDataset('97th', '#88f', rawData['97']));
  return result;
};

/**
 * Gets the percentile data for the selected chart type from the database.
 */
GrowthTracker.Chart.getPercentileData = function() {
  /* get drop-down value */
  var selector = document.getElementById('chartType');
  var chartType = selector.options[selector.selectedIndex].value;
  /* get canvas context */
  var canvas = document.getElementById('chart');
  var ctx = canvas.getContext('2d');

  /* XMLHttpRequest params */
  var url = 'chart.php?action=percentile&gender=f&type=' + chartType;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);
      GrowthTracker.Chart.data.datasets = GrowthTracker.Chart.createDatasets(result);
      GrowthTracker.Chart.chart = new Chart(ctx).Line(GrowthTracker.Chart.data, GrowthTracker.Chart.options);
    }
  };

  xhr.open('GET', url);
  xhr.send();
};