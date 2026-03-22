#
# AdaptixC2 RDLL loader built upon CrystalPalace https://tradecraftgarden.org/crystalpalace.html
#

x64:
	load "bin/loader.x64.o"
		make pic +gofirst +optimize +disco

	load "bin/util.x64.o"
        merge
	
	dfr "resolve" "ror13"
		mergelib "../../cp/lib/libtcg.x64.zip"

	push $DLL
		link "dll"
	
	load "bin/free.x64.o"
		make object
		export
		link "pico"
		
	export
