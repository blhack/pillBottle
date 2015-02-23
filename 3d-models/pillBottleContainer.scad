module pillBox() {
	difference() {
		cube([28,66,16], centered=true);
		translate([3,3,3]) {
			cube([22,60,20], centered=true);
		}
	}
}

pillBox();