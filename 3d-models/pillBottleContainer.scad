module pillBox() {
	difference() {
		cube([25,88,13], centered=true);
		translate([1.5,1.5,1.5]) {
			cube([22,85,20], centered=true);
		}
	}
	translate([0,25,0]) {
		difference() {
			cube([25,1.5,13]);
			translate([10,-.1,0]) {
				cube([5,2,14]);
			}
		}
	}		
}

module lid() {
	translate([-5,0,0]) {
		cube([1.5,88,25]);
	}
}

module usbDoor() {
	translate([6.5,85,6]) {
		cube([12,5,7.5]);
	}
}

difference() {
	pillBox();
	usbDoor();
}

lid();