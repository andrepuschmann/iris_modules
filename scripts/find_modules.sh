#!/bin/bash


DIRS="components controllers"
confname=config/found_modules.m4
project_root=$(pwd)

# check if $1 is a module directory
is_module()
{
	curabsdir=$(pwd)
	curfulldir=$(echo $curabsdir | sed "s|$project_root/||")/$1
	
	firstsubdir=$(echo $curfulldir | awk -F/ '{print $1}')
	if test "x$firstsubdir" = "xcomponents" ; then
		suffix=Component
	elif test "x$firstsubdir" = "xcontrollers" ; then
		suffix=Controller
	else
		return 1
	fi 
	
	if test -e "$1/$1$suffix.cpp" ; then
		compfile="$1/$1$suffix.cpp"
	else
		compfile=$(ls $1/$1$suffix_*x.cpp 2> /dev/null | awk 'NR == 1 {print}')
	fi
	#echo "this is $compfile"
		
	(test -d "$1") && (test -e "$1/Makefile.am") && \
	(test -e "$compfile") && (grep "iris" "$compfile" &> /dev/null) && \
	   return 0

	return 1
}


parse_subdirs()
{
	(
	cd $1
	local subdirs=$(ls -l | awk '$1 ~ /^d/ {print $NF}')
	echo "SUBDIRS = " > Makefile.subdirs
	for subdir in $subdirs ; do
		if test -e $subdir/Makefile.am ; then
			local curabsdir=$(pwd)
			local curfulldir=$(echo $curabsdir | sed "s|$project_root/||")/$subdir
			if is_module $subdir ; then 
				echo "$curfulldir"
				local conditional=$(echo ${curfulldir} | sed "s/\//_/g" | tr [a-z] [A-Z])
				echo "if $conditional" >> Makefile.subdirs
			fi
			echo "SUBDIRS += $subdir" >> Makefile.subdirs
			if is_module $subdir ; then
				echo "endif" >> Makefile.subdirs
			else
				echo $curfulldir/Makefile >> $project_root/$confname
			fi
		fi
		# don't go further if we found the vcproj and Makefile.am file
		if !(is_module $subdir) ; then
			parse_subdirs $subdir
		fi
	done
	# remove Makefile.subdirs if we are in a Template* directory
	case $1 in
		Template*)
			rm -f Makefile.subdirs
			;;
	esac
	)
}

## Main starts here

# make sure we are in the right directory
if !((test -e ./bootstrap) && (test -d ./components) && \
     (test -d ./controllers) && (test -e Makefile.am) && \
     (test -d ./config)) ; then
	echo "Error: not in the modules root directory." >&2
	exit 1
fi

echo "checking for exising modules..."

test -e $confname && rm $confname
touch $confname

echo "# Macro for adding all found modules to the build" >> $confname
echo "AC_DEFUN([IRIS_FOUND_MODULES], [" >> $confname

echo "AC_CONFIG_FILES([" >> $confname

comps=$(parse_subdirs components)
conts=$(parse_subdirs controllers)

echo "])" >> $confname



for module in $comps $conts ; do
	echo "found: $module" 
	echo "IRIS_BUILD_MODULE([$module])" >> $confname
	# if a test exists, add it to the config files
	if test -d $module/Test && test -e $module/Test/Makefile.am ; then
		echo "AC_CONFIG_FILES([$module/Test/Makefile])" >> $confname
	fi
done

echo "done."

echo "checking for files in lib/windows..."
(
cd lib/windows
files=$(svn list -R --non-interactive 2> /dev/null | sed "s/.*\/$//")
# leave file as is if the above command fails
if test "x$files" != "x" ; then 
	echo "EXTRA_DIST = " $files > Makefile.dist
fi
# if file does not exist, create an emtpy version and warn
if test ! -e Makefile.dist ; then
	echo "warning: svn list does not seem to work, creating empty lib/windows/Makefile.dist"
	touch Makefile.dist
fi
)
echo "done."

echo "checking for libraries to be built..."
(
if test -e "lib/linux/usrp" && test -d "lib/linux/usrp" ; then
  echo "lib/linux/usrp"
  echo "if BUILD_USRP" > lib/linux/Makefile.subdirs
  echo "  SUBDIRS = usrp" >> lib/linux/Makefile.subdirs
	echo "endif" >> lib/linux/Makefile.subdirs
	
	# make sure the GRC_USRP macro gets called in any case (even if no usrp comp is built)
	echo "# if none of usrptx or usrprx are enabled, we need to call GRC_USRP anyway" >> $confname
	echo "# to set the AM_CONDITIONALS" >> $confname 
	echo "if test \"x\$iris_cv_var_usrp_passed\" = \"x\" ; then" >> $confname
	echo "  # we have not called GRC_USRP yet" >> $confname
	echo "  enable_usrp=no" >> $confname
	echo "  GRC_USRP([],[])" >> $confname
	echo "fi" >> $confname
	
	test -e config/grc_usrp_dummy.m4 && rm -f config/grc_usrp_dummy.m4
else
	echo "" > lib/linux/Makefile.subdirs
	
	# add a dummy GRC_USRP function, which simply calls the failed function
	echo "AC_DEFUN([GRC_USRP],[" > config/grc_usrp_dummy.m4
	echo "ifelse([\$2], [], :, [\$2])" >> config/grc_usrp_dummy.m4
	echo "])" >> config/grc_usrp_dummy.m4
fi
)
echo "done."

echo "])" >> $confname



