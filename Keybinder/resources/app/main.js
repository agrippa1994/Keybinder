var app = require('app');
var BrowserWindow = require('browser-window');

// Quit when all windows are closed and no other one is listening to this.
app.on('window-all-closed', function() {
  if (app.listeners('window-all-closed').length == 1)
    app.quit();
});

// Create default menu.
app.once('ready', function() {
    
});

// 
require('./Keybinder.js');
