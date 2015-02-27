module tray() {
	translate([1.5,1.5,1.5]) {
		cube([66,45,10], centered=true);	
	}
}

module walls() {
	cube([69,48,10], centered=true);
}

module batteryHole() {
	translate([-1.5,1.5,1.5]) {
		cube([5,16,10]);
	}
}

module holder() {
	difference() {
		difference() {
			walls();
			batteryHole();
		}
		tray();
	}
}

holder();