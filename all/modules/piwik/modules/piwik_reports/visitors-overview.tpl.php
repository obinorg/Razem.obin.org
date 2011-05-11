<?php
// $Id: visitors-overview.tpl.php,v 1.1.2.5 2009/06/16 10:54:56 hass Exp $
?>
<h2><?php
  switch ($period) {
    case 'week':
      print t('Visitors in time period by week');
      break;

    default:
      print t('Visitors in time period by day');
  }
?></h2>
<!-- 
<div id="VisitsSummarygetLastVisitsGraphChart">
  <iframe width="100%" height="200" src="<?php print $widget1_url ?>" scrolling="no" frameborder="0" marginheight="0" marginwidth="0"></iframe>
</div>
 -->
<div class="content">
  <object width="100%" height="300" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="VisitsSummarygetLastVisitsGraphChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>
