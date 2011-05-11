
----------------------------------
ADVANCED CSS/JS AGGREGATION MODULE
----------------------------------

CONTENTS OF THIS FILE
---------------------

 * Fast 404
 * Features & benefits
 * Configuration
 * Technical Details & Hooks
 * Single htaccess rules

FAST 404
--------

Assuming that this guide was followed:
http://2bits.com/drupal-planet/reducing-server-resource-utilization-busy-sites-implementing-fast-404s-drupal.html
and your having issues with Advanced CSS/JS Aggregation. Advagg works similar
to imagecache, thus we need to add in an exceptions for the directories that
advagg uses. Replace this if statement

    if (!strpos($_SERVER['QUERY_STRING'], 'imagecache')) {

with one like this:

    if (!strpos($_SERVER['QUERY_STRING'], 'imagecache') && !strpos($_SERVER['QUERY_STRING'], '/advagg_')) {

This will most likely be in your settings.php file. If you are still having
problems, open an issue on the advagg issue queue:
http://drupal.org/project/issues/advagg

FEATURES & BENEFITS
-------------------

Advanced CSS/JS Aggregation Core Module:
 * Imagecache style CSS/JS Aggregation. If the file doesn't exist it will be
   generated on demand.
 * Stampede protection for CSS and JS aggregation. Uses locking so multiple
   requests for the same thing will result in only one thread doing the work
   while the others wait around for it to be completed.
 * Zero file I/O if the Aggregated file already exists. Results in better page
   generation performance.
 * Smarter aggregate deletion. CSS/JS aggregates only get removed from the cache
   if they have not been accessed in the last 30 days.
 * Smarter cache flushing. Scans all CSS/JS files that have been added to any
   aggregate; if that file has changed then rebuild all aggregates that contain
   the updated file and give the newly aggregated file a new name. The new name
   ensures changes go out when using CDNs.
 * Works with Drupal's private file system. Can Use a separate directory for
   serving aggregated files from.
 * Footer JS gets aggregated as well.
 * Url query string to turn off aggregation for that request. ?advagg=0 will
   turn off file aggregation if the user has the "bypass advanced aggregation"
   permission. ?advagg=-1 will completely bypass all of Advanced CSS/JS
   Aggregations modules and submodules.
 * Gzip support. All aggregated files can be pre-compressed into a .gz file and
   served from Apache. This is faster then gzipping the file on each request.
 * IE Unlimited CSS support. If using ?advagg=0 the CSS output will change
   to use @import style in order to get around the 31 CSS files limit in IE.
 * CDN support. Advagg integrates with this module.
 * jQuery Update support. Advagg integrates with this module.
 * LABjs support. Advagg integrates with this module.
 * One year browser cache lifetimes for all aggregated files. This is a good
   thing.
 * Drush support. "cc advagg" will issue a smart cache flush.
 * Admin menu support. Cache flushing Advanced CSS/JS Aggregation is available
   in the "Flush all caches" menu.

Advanced CSS/JS Aggregation Submodules:
 * CSSTidy library support. Can compress the generated CSS files with the
   CSSTidy library.
 * jsmin+ library support. Can compress the generated JS files with the jsmin+
   library.
 * Google Libraries API. Load jquery.js & jquery-ui.js from Google's CDN network.
   This is a good thing.

CONFIGURATION
-------------

Settings page is located at:
admin/settings/advagg
 * Enable Advanced Aggregation. You can disable the module here. Same affect as
   placing ?advagg=-1 in the URL.
 * Generate CSS/JS files on request (async mode). If advagg doesn't have a route
   back to its self and this is enabled then you will have a broken site. But
   this enabled one can expect much quicker page generation times after a cache
   flush.
 * Gzip CSS/JS files. For every Aggregated file generated, this will create a
   gzip version of that and then serve that out if the browser accepts gzip
   compression.
 * Generate .htaccess files in the advagg_* dirs. If your using the rules
   located at the bottom of this document in your webroots htaccess file then
   you can disable this checkbox.
 * Use a different directory for storing advagg files. Only available if your
   using a private file system. Allows you to save the generated aggregated
   files in a different directory. This gets around the private file system
   restrictions. If boost is installed, you can safely use the cache directory.
 * File Checksum Mode. Keep at mtime; only use md5 if file modification time is
   unreliable on your setup.
 * IP Address to send all asynchronous requests to. If you wish to have one
   server generate all CSS/JS aggregated files then this allows for that to
   happen.
 * Debug to watchdog. This will output A LOT of data to watchdog. Don't turn on
   unless your trying to debug an issue.
 * Smart cache flush button. Scan all files referenced in aggregated files. If any of
   them have changed, increment the counters containing that file and rebuild
   the bundle.
 * Cache Rebuild button. Recreate all aggregated files. Useful if JS or CSS
   compression was just enabled.
 * Forced Cache Rebuild. Recreate all aggregated files by incrementing internal
   counter for every bundle. One should never have to use this option.
 * Rebuild htaccess files. Recreate the generated htaccess files.

Additional information is available at:
admin/settings/advagg/info
 * Hook Theme Info. Displays the preprocess_page order. Used for debugging.
 * CSS files. Displays how often a files checksum has changed and any data
   stored about it.
 * JS files. Displays how often a files checksum has changed and any data
   stored about it.

TECHNICAL DETAILS & HOOKS
-------------------------

Technical Details:
 * There are 2 database tables and one cache table used by advagg.
 * Files are generated by this pattern: css_[MD5]_[Counter].css
 * In the future support for cache bundles will be added.
 * Every JS file is tested for compressibility. This is necessary because jsmin+
   can bomb on certain files; this allows us to catch these bad files and mark
   them. This doesn't give us 100% success but it is better then nothing.

Hooks:
 * hook_advagg_css_alter. Modify the data before it gets written to the file.
   Useful for compression.
 * hook_advagg_css_pre_alter. Modify the raw $variables['css'] before it gets
   processed. Useful for file replacement.
 * hook_advagg_css_extra_alter. Allows one to set the a prefix and suffix to be
   added into the HTML DOM. Useful for CSS conditionals.
 * hook_advagg_js_alter. Modify the data before it gets written to the file.
   Useful for compression.
 * hook_advagg_js_pre_alter. Modify the raw $javascript before it gets
   processed. Useful for file replacement.
 * hook_advagg_js_extra_alter. Allows one to set the a prefix and suffix to be
   added into the HTML DOM.
 * hook_advagg_filenames_alter. Allows for a one to many relationship. A single
   request for a bundle name can result in multiple bundles being returned.
 * hook_advagg_files_table. Allows for modules to mark a file as expired.
 * hook_advagg_js_header_footer_alter. Allows one to move JS from the header to
   the footer. Also one can look at both header and footer JS arrays before they
   get processed.

JS/CSS Theme Override:

    $conf['advagg_css_render_function'] - advagg_unlimited_css_builder
    $conf['advagg_js_render_function'] - advagg_js_builder

JS/CSS File Save Override:

    $conf['advagg_file_save_function'] - advagg_file_saver

SINGLE HTACCESS RULES
---------------------

If the directory level htaccess rules are interfering with your server, you can
place these rules in the Drupal roots htaccess file. Place these rules after
"RewriteRule ^(.*)$ index.php?q=$1 [L,QSA]" but before "</IfModule>"


  # Rules to correctly serve gzip compressed CSS and JS files.
  # Requires both mod_rewrite and mod_headers to be enabled.
  <IfModule mod_headers.c>
    # Serve gzip compressed CSS/JS files if they exist and client accepts gzip.
    RewriteCond %{HTTP:Accept-encoding} gzip
    RewriteCond %{REQUEST_URI} (^/(.+)/advagg_(j|cs)s/(.+)\.(j|cs)s) [NC]
    RewriteCond %{REQUEST_FILENAME}\.gz -s
    RewriteRule ^(.*)\.(j|cs)s$ $1\.$2s\.gz [QSA]

    # Serve correct content types, and prevent mod_deflate double gzip.
    RewriteRule \.css\.gz$ - [T=text/css,E=no-gzip:1]
    RewriteRule \.js\.gz$ - [T=text/javascript,E=no-gzip:1]

    <FilesMatch "\.(j|cs)s\.gz$">
      # Serve correct encoding type.
      Header append Content-Encoding gzip
      # Force proxies to cache gzipped & non-gzipped css/js files separately.
      Header append Vary Accept-Encoding
    </FilesMatch>
  </IfModule>


You also need to place these rules at the very end of your htaccess file, after
"</IfModule>".


<FilesMatch "^(j|cs)s_[0-9a-f]{32}_.+\.(j|cs)s.*">
  FileETag None
  <IfModule mod_expires.c>
    # Enable expirations.
    ExpiresActive On

    # Cache all aggregated CSS/JS files for 1 year after access (A).
    ExpiresDefault A31556926
  </IfModule>
  <IfModule mod_headers.c>
    # Unset unnecessary headers.
    Header unset ETag
    Header unset Last-Modified
    Header unset Pragma

    # Make these files publicly cacheable.
    Header append Cache-Control "public"
  </IfModule>
</FilesMatch>


Be sure to disable the "Generate .htaccess files in the advagg_* dirs" setting
on the admin/settings/advagg page after placing these rules in the webroots
htaccess file. This is located at the same directory level as Drupal's
index.php.
