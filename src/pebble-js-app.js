Pebble.addEventListener('ready', function() {
	initHooks();
});

function initHooks() {
	Pebble.addEventListener('showConfiguration', function(e) {
		Pebble.openURL('destiny-phatsk.rhcloud.com/jade-rabbit.php');
	});

	Pebble.addEventListener('webviewclosed', function(e) {
		console.log('Configuration window returned: ' + e.response);
	});
}
