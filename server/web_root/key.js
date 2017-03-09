/* globals $, _ */
/* eslint no-tabs: 0 */

$(document).ready(function() {
	// Stop the page from doing a stretch from the top when dragged ;
	document.ontouchmove = function(event) {
		event.preventDefault();
	};

	// Move beyond the address  bar to hide ;
	window.scrollTo(0, 1);

	// Start button click code;
	$("#fwd").mousedown(function() {
		$.post("/save", { key_pressed: "fwd" });
	});
	$("#fwd").mouseup(function() {
		$.post("/save", { key_pressed: "stop" });
	});

	$("#rev").mousedown(function() {
		$.post("/save", { key_pressed: "rev" });
	});
	$("#rev").mouseup(function() {
		$.post("/save", { key_pressed: "stop" });
	});

	$("#neutral").click(function() {
		$.post("/save", { key_pressed: "neutral" });
	});

	$("#left").click(function() {
		$.post("/save", { key_pressed: "left" });
	});

	$("#right").click(function() {
		$.post("/save", { key_pressed: "right" });
	});

	$("#power").change(function() {
		$.post("/save", { key_pressed: "power_" + $(this).val() });
	});

	function keyDownPostEvent(event) {
		if (event.keyCode === 37) {
			$.post("/save", { key_pressed: "L" });
		}
		if (event.keyCode === 38) {
			$.post("/save", { key_pressed: "F" });
		}
		if (event.keyCode === 39) {
			$.post("/save", { key_pressed: "R" });
		}
		if (event.keyCode === 32) {
			$.post("/save", { key_pressed: "N" });
		}
		if (event.keyCode === 87) {
			$.post("/save", { key_pressed: "W" });
		}
		if (event.keyCode === 83) {
			$.post("/save", { key_pressed: "S" });
		}
		if (event.keyCode === 65) {
			$.post("/save", { key_pressed: "A" });
		}
		if (event.keyCode === 68) {
			$.post("/save", { key_pressed: "D" });
		} else if (event.keyCode === 40) {
			$.post("/save", { key_pressed: "B" });
		}
	}

	function keyUpPostEvent(event) {
		if (event.keyCode === 38) {
			$.post("/save", { key_pressed: "Q" });
		} else if (event.keyCode === 40) {
			$.post("/save", { key_pressed: "Q" });
		} else if (event.keyCode === 37) {
			$.post("/save", { key_pressed: "Q" });
		} else if (event.keyCode === 39) {
			$.post("/save", { key_pressed: "Q" });
		}
	}

	$(document).on('keydown', _.throttle(keyDownPostEvent, 250));
	$(document).on('keyup', _.throttle(keyUpPostEvent, 250));
});
