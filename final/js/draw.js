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
};

window.onresize = function() {
  GrowthTracker.Chart.options.width = GrowthTracker.Chart.calculateWidth();
  GrowthTracker.Chart.options.height = GrowthTracker.Chart.options.width * 0.8;
  GrowthTracker.Chart.chart.draw(GrowthTracker.Chart.view, GrowthTracker.Chart.options);
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
      arr[i][j] = parseFloat(arr[i][j]);
    }
    if (i % 3 == 0) arr[i].push(20.2);
    else arr[i].push(null);
  }
  data.addRows(arr);
  return data;
};

/**
 * Gets the percentile data for the selected chart type from the database.
 */
GrowthTracker.Chart.getPercentileData = function() {
  /* get drop-down value */
  var selector = document.getElementById('chartType');
  var chartType = selector.options[selector.selectedIndex].value;
  var chartName = selector.options[selector.selectedIndex].text;
  var chartDiv = document.getElementById('chartDiv');
  var width = GrowthTracker.Chart.calculateWidth();
  var unit = chartType == 'weight' ? 'Kilograms' : 'Centimeters';

  /* determine size */

  /* XMLHttpRequest params */
  var url = 'chart.php?action=chart&gender=f&type=' + chartType;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = JSON.parse(this.responseText);
      GrowthTracker.Chart.data = GrowthTracker.Chart.prepareData(result);
      /* stop if there was a problem loading google library */
      if (!GrowthTracker.Chart.loaded) return;
      GrowthTracker.Chart.view = new google.visualization.DataView(GrowthTracker.Chart.data);
      GrowthTracker.Chart.view.hideColumns([1,2,4,5,7,9,11,12,14,15]);
      GrowthTracker.Chart.options = {
        title: chartName + ' Percentiles',
        curveType: 'function',
        width: width,
        height: width * 0.8,
        chartArea: {
          left:25,top:20,width:'80%', height:'80%'
        },
        series: {
          0: {targetAxisIndex: 0, lineWidth: 1},
          1: {targetAxisIndex: 0, lineWidth: 1},
          2: {targetAxisIndex: 0, lineWidth: 1},
          3: {targetAxisIndex: 0, lineWidth: 1},
          4: {targetAxisIndex: 0, lineWidth: 1},
          5: {targetAxisIndex: 0, pointShape: 'circle', pointSize: 5, color: 'black'}
        },
        hAxis: {
          showTextEvery: 3
        },
        vAxes: {
          0: {title: chartName + ' in ' + unit}
        }
      };
      GrowthTracker.Chart.chart = new google.visualization.LineChart(chartDiv);
      GrowthTracker.Chart.chart.draw(GrowthTracker.Chart.view, GrowthTracker.Chart.options);
    }
  };

  xhr.open('GET', url);
  xhr.send();
};

GrowthTracker.Chart.calculateWidth = function() {
  var chartDiv = document.getElementById('chartDiv');
  return chartDiv.parentNode.clientWidth * 0.99;
};