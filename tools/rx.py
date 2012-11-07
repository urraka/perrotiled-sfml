import sys, os, json

if len(sys.argv) < 2:
	print "Usage: " + sys.argv[0] + " <input.json> <outputdir>"
	sys.exit(1)

inputFile = sys.argv[1]
outputDir = sys.argv[2]
scriptDir = os.path.dirname(sys.argv[0])
rxhFile = os.path.join(scriptDir, "rx.h")
rxcppFile = os.path.join(scriptDir, "rx.cpp")

# files checking

if not os.path.exists(inputFile):
	print "Error: the input file doesn't exist"
	sys.exit(1)

if not os.path.isdir(outputDir):
	print "Error: the output directory doesn't exist"
	sys.exit(1)

if not os.path.exists(rxhFile):
	print "Error: missing template file 'rx.h'"
	sys.exit(1)

if not os.path.exists(rxcppFile):
	print "Error: missing template file 'rx.cpp'"
	sys.exit(1)

print 'Generating rx.cpp & rx.h...'

# read json file

data = None
with open(inputFile) as f:
	data = json.load(f)

# process textures

texturesInit = []
texturesEnum = []

if data.has_key('textures') and type(data['textures']) is dict:
	for k in data['textures'].keys():
		tx = data['textures'][k]
		filename = '"' + tx['filename'].replace('\\', r'\\').replace('"', r'\"') + '"'
		origin = tx['origin'] if tx.has_key('origin') else "0,0"
		size = tx['size'] if tx.has_key('size') else "0,0"

		texturesEnum.append(k)
		texturesInit.append("textures[" + k + "] = Texture(" + filename + ", Vector2i(" + origin + "), Vector2u(" + size + "));")

# process animations

animationsInit = []
animationsEnum = []

if data.has_key('animations') and type(data['animations']) is dict:
	for k in data['animations'].keys():
		anim = data['animations'][k]
		defaultDuration = (("%#.2f" % anim['duration']) + 'f') if anim.has_key('duration') else '0.0f'

		animationsEnum.append(k)

		for frame in anim['frames']:
			duration = (("%#.2f" % frame['duration']) + 'f') if frame.has_key('duration') else defaultDuration
			texture = frame['texture'].split(',')
			textureid = texture[0] if texture[0] != '*' else '-1'
			imageIdx = texture[1] if len(texture) > 1 else '0'
			
			animationsInit.append("animations[" + k + "].frames.push_back(Frame(" + textureid + ", " + duration + ", " + imageIdx + "));")

texturesInit = '\n\t\t'.join(texturesInit)
texturesEnum = ',\n\t\t'.join(texturesEnum) + (',\n\t\t' if len(texturesEnum) else '')

animationsInit = '\n\t\t'.join(animationsInit)
animationsEnum = ',\n\t\t'.join(animationsEnum) + (',\n\t\t' if len(animationsEnum) else '')

rxhContents = None
rxcppContents = None

with open(rxhFile) as f:
	rxhContents = f.read()

with open(rxcppFile) as f:
	rxcppContents = f.read()

rxhContents = rxhContents.replace('/*--TEXTURES--*/', texturesEnum).replace('/*--ANIMATIONS--*/', animationsEnum)
rxcppContents = rxcppContents.replace('/*--INITIALIZE--*/', texturesInit + '\n\n\t\t' + animationsInit)

with open(os.path.join(outputDir, 'rx.h'), 'w') as f:
	f.write(rxhContents)

with open(os.path.join(outputDir, 'rx.cpp'), 'w') as f:
	f.write(rxcppContents)
