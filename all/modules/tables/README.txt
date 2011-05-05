$Id: README.txt,v 1.3 2009/09/10 06:10:27 webgeer Exp $

Description
-----------

The tables module is a filter that converts a [table  ] macro into HTML
encoded table.

Installation
------------

1) copy the tables directory into the modules directory

2) enable the 'module' in drupal

3) configure an 'input format' so that the tables filter is activated.  If
   this will be on a 'html filtered' format, ensure that the weighting is
   such that the HTML filter comes before the tables.

Optionally you can copy the contents of the tables.css into your theme's css
sheet.  Then you should change the settings for the tables module to be blank
for the style sheet.

Instructions
------------

The macro has the format where it begins "[table=class" where class is
a style sheet class name.  If the class is left blank then the default
class as defined in "admin/settings/tables" will be used.

If class is set to theme then a table will be created using the theme
functions of drupal.

cells are separated with a "|" symbol and rows are separated with a newline.
A "]" is used to end the table.

Some special characters can be used.  A "{" will merge the cell with the cell
to the left to create a multi-column cell, a "^" will merge the cell with the
cell above to create a multi-row cell.  A "!" will set the cell to be a
"<th>" rather than the standard "<td>" cell.

In order to create fancy css effects the first row <tr> has a class of
"firstrow" and the other rows all are either "oddrow" or "evenrow".
Similarly, the cells (<td> or <th>) have a class of either "firstcol",
"evencol" or "oddcol".  Using these classes interesting effects can be
achieved.  See the css sheet and the demo page for more information.

Settings
--------

The settings options are the style sheet which is initially set to:
"modules/tables/tables.css" if you want to move the style sheet somewhere
else you should change this.  If you incorporate the table styles into some
other style sheet that is already loaded, you can leave this blank and no
style sheet will be loaded.

The default style is what is used for all tables that do not have a class
set.

Demo
----

See:  http://www.webgeer.com/table_demo

Credit
------

Written by:
James Blake
http://www.webgeer.com/James
