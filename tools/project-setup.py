import sys, os, uuid, re

templateName = 'SFMLGameTemplate'

vc10 = '../vc10/'

# remove user-specific and generated files

if os.path.exists(vc10 + templateName + '.suo'):
	os.remove(vc10 + templateName + '.suo')

if os.path.exists(vc10 + templateName + '.vcxproj.user'):
	os.remove(vc10 + templateName + '.vcxproj.user')

# check that template files exist

if (not os.path.exists(vc10 + templateName + '.sln') or
	not os.path.exists(vc10 + templateName + '.vcxproj') or
	not os.path.exists(vc10 + templateName + '.vcxproj.filters')):
	print 'Error: some of the following files don\'t exist:\n'
	print '\t' + vc10 + templateName + '.sln'
	print '\t' + vc10 + templateName + '.vcxproj'
	print '\t' + vc10 + templateName + '.vcxproj.filters'
	raw_input()
	exit(1)

# these variables will hold the contents of each file (gamecore.sln, gamecore.vcxproj, gamecore.vcxproj.filters)

sln = None
prj = None
flt = None

with open(vc10 + templateName + '.sln') as f:
	sln = f.read()
with open(vc10 + templateName + '.vcxproj') as f:
	prj = f.read()
with open(vc10 + templateName + '.vcxproj.filters') as f:
	flt = f.read()

# search all GUIDs

guids = set()
pattern = re.compile('[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}', re.IGNORECASE)

for fileContents in [sln, prj, flt]:
	for guid in pattern.findall(fileContents, re.IGNORECASE):
		guids.add(uuid.UUID(guid))

guids = list(guids)

# replace all guids with new ones

for guid in guids:
	newGuid = uuid.uuid1()
	pattern = re.compile(re.escape(str(guid)), re.IGNORECASE)
	replacement = str(newGuid).upper()
	sln = pattern.sub(replacement, sln)
	prj = pattern.sub(replacement, prj)
	flt = pattern.sub(replacement, flt)

# request a new name for the project

newName = raw_input('Enter the new project name: ').strip()
pattern = re.compile(re.escape(templateName))
sln = pattern.sub(newName, sln)
prj = pattern.sub(newName, prj)
flt = pattern.sub(newName, flt)

# check that files don't exist first to avoid overwriting

if (os.path.exists(vc10 + newName + '.sln') or
	os.path.exists(vc10 + newName + '.vcxproj') or
	os.path.exists(vc10 + newName + '.vcxproj.filters')):
	print 'Error: files with the project name you chose already exist.'
	raw_input()
	exit(1)

# save the new project files

userFile = """<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LocalDebuggerWorkingDirectory>$(ProjectDir)..\</LocalDebuggerWorkingDirectory>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LocalDebuggerWorkingDirectory>$(ProjectDir)..\</LocalDebuggerWorkingDirectory>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
</Project>"""

with open(vc10 + newName + '.sln', 'w') as f:
	f.write(sln)
with open(vc10 + newName + '.vcxproj', 'w') as f:
	f.write(prj)
with open(vc10 + newName + '.vcxproj.filters', 'w') as f:
	f.write(flt)
with open(vc10 + newName + '.vcxproj.user', 'w') as f:
	f.write(userFile)

print 'Project files created in vc10/ directory.'
raw_input()
