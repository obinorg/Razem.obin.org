// $Id: piwik_reports.js,v 1.1.2.9 2009/06/13 13:19:43 hass Exp $

Drupal.behaviors.piwik_reports = function() {

  var url = $("#edit-url").val();
  var page = $("#edit-page").val();

  // Build HTML for data table.
  var columns = 2;
  var header = "<table class='sticky-enabled sticky-table'><thead class='tableHeader-processed'>";
  header += "<tr><th>" + Drupal.t('Label') + "</th>";
  switch (page) {
    case "actions":
      header += "<th>" + Drupal.t('Visitors') + "</th><th>" + Drupal.t('Hits') + "</th>";
      columns = 3;
      break;
    case "websites":
      header += "<th>" + Drupal.t('Visitors') + "</th>";
      break;
    case "search":
      header += "<th>" + Drupal.t('Visits') + "</th>";
      break;
  }
  header += "</tr></thead><tbody></tbody></table>";

  // Add the table and show "Loading data..." status message for long running requests.
  $("#pagestable").html(header);
  $("#pagestable > table > tbody").html("<tr class='odd'><td colspan='" + columns + "'>" + Drupal.t('Loading data...') + "</td></tr>");

  // Get data from remote Piwik server.
  $.getJSON(url, function(data){
    var content = "";
    var tr_class = "even";

    $.each(data, function(i,item){
      if (tr_class == "odd") { item_class = "even"; } else { item_class = "odd"; }
      tr_class = item_class;

      content += "<tr class='" + item_class + "'><td>" + item["label"] + "</td>";
      switch (page) {
        case "actions":
          content += "<td>" + item["nb_visits"] + "</td><td>" + item["nb_hits"] + "</td>";
          break;
        case "websites":
          content += "<td>" + item["nb_visits"] + "</td>";
          break;
        case "search":
          content += "<td>" + item["nb_visits"] + "</td>";
          break;
      }
      content += "</tr>";
    });

    // Push data into table and replace "Loading data..." status message.
    if (content) {
      $("#pagestable > table > tbody").html(content);
    }
    else {
      $("#pagestable > table > tbody > tr.odd > td").html(Drupal.t('No data available.'));
    }
  });

};