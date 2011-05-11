<?php

// $Id: views_handler_filter_support_client.inc,v 1.1 2009/08/03 11:45:08 dubs Exp $

class views_handler_filter_support_client extends views_handler_filter_in_operator {
	function get_value_options() {
		if (isset($this->value_options)) {
		  return;
		}
		$this->value_options = array();
		$result=db_query("SELECT clid, name FROM support_client");
		while ($row = db_fetch_array($result)) {
			$this->value_options[$row['name']] = t(ucfirst($row['name']));
		}
  	}
}
