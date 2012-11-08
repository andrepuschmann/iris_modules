#!/bin/bash
# This script adds a copyright notice to all files

set -f
notice_cpp=$(cat copyright_notice | awk 'BEGIN {print "/*"} {print " * " $0} END {print " */"}')
notice_m4=$(cat copyright_notice | awk 'BEGIN {print "#"} {print "# " $0}')

# awk script to extract the copyright notice of a C/C++ file
extract_notice_cpp_awk='
BEGIN {
	start=0;
	copy=0;
	ready=0;
	startline=0
	endline=0
	str="";
	content=""
}

$0 ~ /.*\/\*.*/ && !ready && !start { start=1 ; str="" ; startline=NR} 
$0 ~ /.*[C|c]opyright.*/ && start {copy=1 } 
start {str = str $0 "\n" } 
$0 ~/.*\*\/.*/ && start { start=0 ; if (copy) {ready=1 ; endline=NR } } 

END {
	if (str != "") {
		printf "%s", str;
		printf "Lines %d:%d\n", startline, endline+1;
	}
	else
		exit 1
}
'

# awk script to extract the copyright notice of a m4/Makefile file
extract_notice_m4_awk='
BEGIN {
	start=0;
	copy=0;
	ready=0;
	startline=0
	endline=0
	str="";
	content=""
}

$0 ~ /^#/ && !ready && !start { start=1 ; str="" ; startline=NR} 
$0 ~ /^#.*[C|c]opyright.*/ && start {copy=1 } 
start {str = str $0 "\n" } 
$0 !~/^#/ && start { start=0 ; if (copy) {ready=1 ; endline=NR } } 

END {
	if (str != "") {
		printf "%s", str;
		printf "Lines %d:%d\n", startline, endline;
	}
	else
		exit 1
}
'


# awk script to replace a notice (define variables start,end, and notice before execution)
replace_notice_awk='
BEGIN {
done = 0;
printed = 0;
}

NR > end { done=1}

{
	if (NR >=start && NR <= end && !done) 
	{
		if (!printed)
		{
			print notice ;
		        printf "\n";	
			printed=1;
		}
	}
	else print
}
'

set +f

if [ "x$notice_cpp" = "x" ] ; then
	echo "could not find the copyright notice" 
	exit 1
fi

# make sure we are in the right directory
if [ ! \( -e "configure.in" \) ] ; then
	cd ..
	if [ ! \( -e "configure.in" \) ] ; then
		echo "could not find configure.in - are you in the right directory?"
		exit 1
	fi
fi

# find all .cpp, .c and .h files (use svn list to make sure files are on svn)
files1=$( svn list -R | grep "\.cpp$")
files2=$( svn list -R | grep "\.h$")
files3=$( svn list -R | grep "\.c$")
files=$(echo -e "$files1\n$files2\n$files3" | grep -v ticpp | grep -vi "/fftw" | \
         grep -v USRPLibrary/firmware | grep -v USRPLibrary/fpga | \
         grep -v USRPLibrary/host | grep -v XGetopt.cpp)


# make a backup of all files
for file in $files ; do
	cp $file $file~
done

# update the copyrights
pattern="^.*\*.*[C|c]opyright"
for file in $files ; do
	if grep -i "$pattern" $file &> /dev/null ; then 
		# there is a copyright already, confirm and replace it
		current=$(awk "$extract_notice_cpp_awk" $file | grep -v "^Lines")
		if echo "$current" | grep -q "Undivided Trinity" ; then
			echo "replacing copyright in $file"
		else
			echo "copyright exists in $file:"
			echo "$current"
			echo -n "Replace? [y/N] "
			read ans
		
			if [ "x$ans" != "xy" ] &&  [ "x$ans" != "xY" ] ; then
				continue
			fi
		fi
		
		lines=$(awk "$extract_notice_cpp_awk" $file | grep Lines | sed "s/Lines //")
		start=$(echo $lines | sed "s/:[0-9]*$//")
		end=$(echo $lines | sed "s/^[0-9]*://")
		awk -v start="$start" -v end="$end" -v notice="$notice_cpp" "$replace_notice_awk" $file > $file.cpy
		mv $file.cpy $file
	else
		echo "insert copyright: $file"
		(echo "$notice_cpp" ; echo -e "\n\n" ; cat $file ) > $file.cpy
		mv $file.cpy $file
	fi
done

# find all .m4 and Makefile.am files
# find all .cpp, .c and .h files (use svn list to make sure files are on svn)
files1=$( svn list -R | grep "\.m4$")
#files2=$( svn list -R | grep ".sh$")
files3=$( svn list -R | grep "Makefile\.am$")
files=$(echo -e "$files1\n$files3" | grep -v ticpp | grep -vi "/fftw" \
        | grep -v ax_create_pkgconfig_info.m4 | grep -v ax_boost.m4 | \
         grep -v USRPLibrary/firmware | grep -v USRPLibrary/fpga | \
         grep -v USRPLibrary/host | grep -v grc_usrp.m4 | grep -v usrp_sdcc.m4 | \
         grep -v usrp_libusb.m4 | grep -v usrp_fusb_tech.m4 | grep -v gr_check_usrp.m4)

# make a backup of all files
for file in $files ; do
	cp $file $file~
done

# update the copyrights
pattern="^#.*[C|c]opyright"
for file in $files ; do
	if grep -i "$pattern" $file &> /dev/null ; then 
		# there is a copyright already, confirm and replace it
		current=$(awk "$extract_notice_m4_awk" $file | grep -v "^Lines")
		if echo "$current" | grep -q "Undivided Trinity"  ; then
			echo "replacing copyright in $file"
		else
			echo "copyright exists in $file:"
			echo "$current"
			echo -n "Replace? [y/N] "
			read ans
		
			if [ "x$ans" != "xy" ] &&  [ "x$ans" != "xY" ] ; then
				continue
			fi
		fi
		
		lines=$(awk "$extract_notice_m4_awk" $file | grep Lines | sed "s/Lines //")
		start=$(echo $lines | sed "s/:[0-9]*$//")
		end=$(echo $lines | sed "s/^[0-9]*://")
		awk -v start="$start" -v end="$end" -v notice="$notice_m4" "$replace_notice_awk"  $file > $file.cpy
		mv $file.cpy $file
	else
		echo "insert copyright: $file"
		(echo "$notice_m4" ; echo -e "\n\n" ; cat $file ) > $file.cpy
		mv $file.cpy $file
	fi
done

