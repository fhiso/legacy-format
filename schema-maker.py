import re

md_file = 'data-model.md'

chunks = re.compile(r'^([A-Za-z ]+?)[ \t]*\n(?:[: ] +([^\n]*)\n)+', re.MULTILINE)
links = re.compile(r'  `\[([^\]]*)\]`')

def curie(s):
    return s#.replace('elf:','https://terms.fhiso.org/elf/').replace('elfm:','https://terms.fhiso.org/elf/metadata/')

themap = {}

with open(md_file) as f:
    txt = f.read()
    bits = re.split(r' *###+ *',txt)
    for bit in bits:
        name = bit[:bit.find('{#')].strip()[1:-1]
        if ' ' in name: continue
        isa = []
        within = []
        contains = []
        canbe = []
        tag = None
        parts = {}
        for chunk in chunks.finditer(bit):
            if chunk.group(1).startswith('Supertype'):
                isa.extend(links.findall(chunk.group()))
            if chunk.group(1).startswith('Superstructure'):
                within.extend(links.findall(chunk.group()))
            if chunk.group(1).startswith('Default tag') and tag is None:
                tag = chunk.group()
                tag = tag[tag.find('`')+1:]
                tag = tag[:tag.find('`')]
            if chunk.group(1).startswith('Subtype'):
                canbe.extend(links.findall(chunk.group()))
            if chunk.group(1).startswith('Substructure'):
                contains.extend(links.findall(chunk.group()))
            parts[chunk.group(1)] = chunk.group()
        themap[name] = {
            'isa': isa,
            'within': within,
            'contains': contains,
            'canbe': canbe,
            'tag': tag,
            'parts': parts,
            'bit': bit
        }
        if 0:
            print('\n\n###',bit)
            if tag is not None:
                print('''
    Schema fragment
    :   `1 SCHMA`
    :   `2 IRI {}`
    :   `3 TAG {} {}`'''.format(
                    curie(name),
                    tag,
                    ' '.join(curie(_) for _ in within)
                    )
                )

from sys import stderr, argv

missing = set()

for k,v in themap.items():
    for p in v['isa']:
        if p not in themap:
            missing.add(p)
        else:
            if k not in themap[p]['canbe']:
                print('Unlisted subtype:',p,'can be a',k, file=stderr)
    for p in v['canbe']:
        if p not in themap:
            missing.add(p)
        else:
            if k not in themap[p]['isa']:
                print('Unlisted supertype:',p,'can be a',k, file=stderr)
    for p in v['within']:
        if p not in themap:
            missing.add(p)
        else:
            if k not in themap[p]['contains']:
                print('Unlisted element:',p,'can contain',k, file=stderr)
    for p in v['contains']:
        if p not in themap:
            missing.add(p)
        else:
            if k not in themap[p]['within']:
                print('Unlisted element:',p,'can be within',k, file=stderr)
print('missing tags:', missing, file=stderr)



for i in range(3): # short-circuit fixed point
    for k,v in themap.items():
        for sup in v['isa']:
            if sup in themap:
                v['within'] = list(sorted(set(v['within'] + themap[sup]['within'])))


if 1:
    import datetime

    print('''0 HEAD
1 CHAR UTF-8
1 GEDC 
2 VERS 5.5.1
2 FORM LINEAGE-LINKED
1 SCHMA
2 PRFX elf https://fhiso.org/elf/
2 PRFX elfm https://fhiso.org/elf/metadata/''')

    for k in sorted(themap):
        if ':' in k:
            print('2 IRI', k)
            for sup in sorted(themap[k]['isa']):
                if sup != 'elf:Structure':
                    print('3 ISA', sup)
            if 'tag' in themap[k] and themap[k]['tag'] is not None:
                print('3 TAG', themap[k]['tag'], ('\n4 CONT ' if False else ' ').join(sorted(themap[k]['within'])))
    print('''1 SOUR https://fhiso.org/elf/
1 NOTE This file was automatically generated from '''+md_file+'''
2 CONT by '''+argv[0]+''' at '''+datetime.datetime.utcnow().isoformat()+'''
1 SUBM @fhiso_elf1@
0 @fhiso_elf1@ SUBM
1 NAME FHISO Extended Legacy Format, version 1
0 TRLR''')
elif 1:
    print('| {:36} | {:30} | {:5} |'.format('$I$', '$S$', '$T$'))
    print('|:'+ '-'*36 +'-|:'+ '-'*30 +'-|:'+ '-'*5 +'-|')
    for k in sorted(themap):
        if 'tag' in themap[k] and themap[k]['tag'] is not None:
            for sup in sorted(themap[k]['within']):
                print('| {:36} | {:30} | {:5} |'.format(k, sup, themap[k]['tag']))
    
else:
    tis = []
    for k in sorted(themap):
        if 'tag' in themap[k] and themap[k]['tag'] is not None:
            for sup in sorted(themap[k]['within']):
                tis.append((themap[k]['tag'], k, sup))
    tis.sort()
    for t,i,s in tis:
        print(t,i)
