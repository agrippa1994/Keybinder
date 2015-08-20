
var app = require('app');
var BrowserWindow = require('browser-window');
var globalShortcut = require('global-shortcut');

var mainWindow = null;

// Quit when all windows are closed.
app.on('window-all-closed', function() {
    app.quit();
});

app.on('ready', function() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600
    });
    
    mainWindow.loadUrl('file://' + __dirname + '/index.html');
    mainWindow.focus();
    //mainWindow.openDevTools();
    
    globalShortcut.register("s", function() {
        //alert("ctrl + s gedrückt");
        return false;
    });
});
