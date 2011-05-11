// $Id: README.txt,v 1.1.2.5 2009/05/01 09:29:27 hass Exp $

Module: Piwik - Web analytics
Author: Alexander Hass <www.hass.de>


Description
===========
Adds the Piwik tracking system to your website.

Requirements
============

* Piwik installation
* Piwik website account


Installation
============
* Copy the 'piwik' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
In the settings page enter your Piwik website ID.

You will also need to define what user roles should be tracked.
Simply tick the roles you would like to monitor.

All pages will now have the required JavaScript added to the
HTML footer can confirm this by viewing the page source from
your browser.


Advanced Settings
=================
You can include additional JavaScript snippets in the advanced
textarea. These can be found on various blog posts, or on the
official Piwik pages. Support is not provided for any customisations
you include.

To speed up page loading you may also cache the piwik.js
file locally. You need to make sure the site file system is in public
download mode.


Known issues
============
Drupal requirements (http://drupal.org/requirements) tell you to configure 
PHP with "session.save_handler = user", but your Piwik installation will
not work with this configuration and gives you a server error 500.

1. You are able to workaround with the PHP default in your php.ini:

   [Session]
   session.save_handler = files

2. With Apache you may overwrite the PHP setting for the Piwik directory only.
   If Piwik is installed in /piwik you are able to create a .htaccess file in
   this directory with the below code:

   # PHP 4, Apache 1.
   <IfModule mod_php4.c>
     php_value session.save_handler files
   </IfModule>

   # PHP 4, Apache 2.
   <IfModule sapi_apache2.c>
     php_value session.save_handler files
   </IfModule>

   # PHP 5, Apache 1 and 2.
   <IfModule mod_php5.c>
     php_value session.save_handler files
   </IfModule>
