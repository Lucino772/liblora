Import('env')

# This selects the appropriate framework for the liblora library
frameworks = env.get('PIOFRAMEWORK', [])
if len(frameworks) == 0:
    raise RuntimeError('Invalid framework')

env.Replace(SRC_FILTER=["+<src*>", "-<src/framweworks*>", "+<src/frameworks/%s>" % frameworks[0]])
