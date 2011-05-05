<?php
// $Id: razem_obin_org_views_handler_field_project.inc,v 1.1 2011/05/05 16:16:21 henk Exp $

class razem_obin_org_views_handler_field_spaces_og extends views_handler_field_spaces_og {

  /**
   * Override this in a subclass to provide custom color generation logic.
   */
  protected function color($value) {
    static $colors;
    if (empty($colors)) {
      $colors = array();
    }
    if (empty($colors['value'])) {
      $designkit_value = unserialize(db_result(db_query("SELECT value FROM {spaces_overrides} WHERE
        type='og' AND id=%d AND object_type='variable' AND object_id='designkit_color'",
        $value
      )));
      if (!empty($designkit_value)) {
        $colors[$value] = $designkit_value['background'];
      }
      else {
        $colors[$value] = $value;
      }
    }
    return $colors[$value];
  }
}
