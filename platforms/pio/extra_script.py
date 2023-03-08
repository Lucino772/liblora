Import('env')

# This selects the appropriate framework for the liblora library
frameworks = env.get('PIOFRAMEWORK', [])
if len(frameworks) == 0:
    raise RuntimeError('Invalid framework')

env.Append(CPPDEFINES=[
  "LIBLORA_FRAMEWORK_{}".format(str(frameworks[0]).upper())
])

env.Append(CDEFINES=[
  "LIBLORA_FRAMEWORK_{}".format(str(frameworks[0]).upper())
])
