import sys
template = open('libread/readme.md.template', 'r').read()
template = template.format(sys.argv[1])
open('readme.md', 'w').write(template)
