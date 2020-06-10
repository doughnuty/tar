TAR PROJECT
QUESTIONS: 
1. Can the return value be different from the one of GNU tar?
2. GNU tar require the archive name to follow exactly after the f 


mods:
(-f required):
______
	-c - Create new containing specified names, directory arcgive recursively
	-r - Append to the archive
	-t - List contents to stdout, if arguments given they specify the names to be listed
	-u - Append (not replace) if modification date newer. Can result into multiple versions of the same file.
	-x - Extract (if same names later copies overwrite). Arguments specify names of elemets to be extracted.
______
	-C - change directory, order-sensitive, affects all options that follow. Take file from another directory, archive it in the working one. Example:
tar -c test2.tar my_file.f -C tar main.c -f test.tar
	-f - archive name

-- to check the work use cat command --
-- on extraction output name of extracted files --
-- file entry = file header (name&stat&chksm) + contents --
-- blocksize = 512 --
WHAT TO DO:

1. Parse arguments (dont forget to free everything if not needed);
-- to write to the particular file you need its file descriptor -
2. Open archive;
3a. Create archive:
{
    4. Create header;
    5. Get the contents: 
    {
	-- wil need struct to hold everything. att1 --
	-- struct stat --
	a) open with filename
	b) read contents 
	c) write contents
	d) write zero bytes (bzero), size 512
    }
}
3b. Extract archive:
{
    4. Create file based on header
    5. open->time->write
}
6. Delete arguments.
7. Return 0 or >0;

Error Messages:

1. If -f was not specified when needed:
tar: Refusing to read archive contents from terminal (missing -f option?)

1a. If f specified but incorrect argument given:
tar: --filename--: Cannot open: No such file or directory

2. If file to archive/append/open is missing:
tar: main.c: Cannot stat: No such file or directory

3. If -C was specified in the end: 
tar: The following options were used after any non-optional arguments in archive create or update mode.  These options are positional and affect only arguments that follow them.  Please, rearrange them properly.
tar: -C ‘..’ has no effect

4. Option specification problem 
tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option
