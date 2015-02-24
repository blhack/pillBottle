module pillBox() {
	difference() {
		cube([25,88,13], centered=true);
		translate([1.5,1.5,1.5]) {
			cube([22,85,20], centered=true);
		}
	}
}

pillBox();