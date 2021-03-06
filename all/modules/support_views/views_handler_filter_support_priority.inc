<?php

// $Id: views_handler_filter_support_priority.inc,v 1.1 2009/08/03 11:45:08 dubs Exp $

class views_handler_filter_support_priority extends views_handler_filter_in_operator {
	function get_value_options() {
		if (isset($this->value_options)) {
		  return;
		}
		$this->value_options = array();
		$result=db_query("SELECT pid, priority FROM support_priority");
		while ($row = db_fetch_array($result)) {
			$this->value_options[$row['priority']] = t(ucfirst($row['priority']));
		}
  	}
}
