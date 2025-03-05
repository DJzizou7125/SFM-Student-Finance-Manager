
{ pkgs }: {
	deps = [
		pkgs.clang
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
		pkgs.cairo
		pkgs.gtk3
	];
}
