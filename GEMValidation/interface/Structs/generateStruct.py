#!/usr/bin/python

## event
event = {
    "int" : ["run","lumi","bx","orbit","time"]
    "unsigned long long" : ["event"]
}

## gen particles
gen = {
    "float" : ["pt","pz","eta","phi","dxy","d0","z0","d0_prod","z0_prod","vx","vy","vz","r"],
    "int" : ["charge","pdgid","tpid","llp_in_acceptance"]
}

sim = {
    "float" : ["pt","px","py","pz","eta","phi","vx","vy","vz","d0","z0","d0_prod","z0_prod"],
    "int" : ["charge", "pdgid", "index", "id_gen"],
    "vint": ["id_gem_sh",
             "id_gem_dg",
             "id_gem_pad",
             "id_gem_copad",
             "id_gem_cluster",
             "id_csc_sh",
             "id_csc_wire",
             "id_csc_strip",
             "id_csc_preclct",
             "id_csc_clct",
             "id_csc_alct",
             "id_csc_lct",
             "id_csc_mplct",
             "id_csc_shower",
             "id_emtf_track",
             "id_emtf_cand",
             "id_emtf_shower",
             "id_l1mu",
             "id_l1mu_shower",
             "id_l1track",
             "id_l1trackmu"]
}

gemsimhit = {
    "int" : ["bx", "strip_l1", "strip_l2", "isodd", "region", "station", "ring", "chamber", "tpid", "nlayers"],
    "float" : ["dphi", "bend", "phi", "eta", "perp"]
}

cscsimhit = {
    "int" : [
        "bx",
        "keyhs",
        "isodd",
        "region",
        "station",
        "ring",
        "chamber",
        "tpid",
        "nlayers",
        "strip_l1",
        "strip_l6",
    ],
    "float" : [
        "dphi",
        "bend",
        "phi",
        "phi_l1",
        "phi_l6",
        "eta",
        "perp",
    ]
}

gemstrip = {
    "int" : [
        "bx",
        "strip",
        "isodd",
        "region",
        "station",
        "roll",
        "layer",
        "chamber",
        "tpid"],
    "float" : [
        "phi",
        "eta",
    ]
}

gempad = {
    "int" : [
        "bx",
        "pad",
        "isodd",
        "region",
        "station",
        "roll",
        "layer",
        "chamber",
        "tpid"],
    "float" : [
        "phi",
        "eta",
    ]
}

gemcopad = {
    "int" : [
        "bx",
        "pad",
        "isodd",
        "region",
        "station",
        "roll",
        "layer",
        "chamber",
        "tpid"],
    "float" : [
        "phi",
        "eta",
    ]
}

gemcluster = {
    "int" : [
        "bx",
        "pad",
        "isodd",
        "size",
        "region",
        "station",
        "roll",
        "layer",
        "chamber",
        "tpid"],
    "float" : [
        "phi",
        "eta",
    ]
}

csccomp = {
    "int" : [
        "time",
        "hs",
        "isodd",
        "region",
        "station",
        "layer",
        "ring",
        "quality",
        "chamber",
        "tpid"],
}

cscstrip = {
    "int" : [
        "bx",
        "strip",
        "isodd",
        "region",
        "station",
        "ring",
        "quality",
        "chamber",
        "tpid"],
}

cscwire = {
    "int" : [
        "time",
        "keywg",
        "isodd",
        "region",
        "station",
        "ring",
        "layer",
        "chamber",
        "tpid"],
}

alct = {
    "int" : [
        "bx",
        "wg",
        "isodd",
        "region",
        "station",
        "ring",
        "sector",
        "chamber",
        "quality",
        "tpid",

    ]
}

preclct = {
    "int" : [
        "bx",
        "hs",
        "qs",
        "es",
        "isodd",
        "region",
        "station",
        "ring",
        "sector",
        "chamber",
        "quality",
        "pattern",
        "pattern_run3",
        "tpid",

    ]
}

clct = {
    "int" : [
        "bx",
        "hs",
        "qs",
        "es",
        "isodd",
        "region",
        "station",
        "ring",
        "sector",
        "chamber",
        "quality",
        "pattern",
        "pattern_run3",
        "tpid",

    ]
}

lct = {
    "int" : [
        "bx",
        "wg",
        "hs",
        "qs",
        "es",
        "isodd",
        "region",
        "station",
        "ring",
        "sector",
        "chamber",
        "quality",
        "pattern",
        "pattern_run3",
        "tpid",

    ]
}

mplct = {
    "int" : [
        "bx",
        "wg",
        "hs",
        "qs",
        "es",
        "isodd",
        "region",
        "station",
        "ring",
        "sector",
        "chamber",
        "quality",
        "pattern",
        "pattern_run3",
        "tpid",
    ]
}

cscshower = {
    "int" : [
        "bx",
        "region",
        "station",
        "ring",
        "chamber",
        "sector",
        "isLooseInTime",
        "isNominalInTime",
        "isTightInTime",
        "isLooseOutOfTime",
        "isNominalOutOfTime",
        "isTightOutOfTime",
    ]
}

emtftrack = {
    "float" : ["pt","eta","phi"],
    "int" : ["charge","bx","tpid"]
}

emtfcand = {
    "float" : ["pt","eta","phi"],
    "int" : ["charge","bx","tpid"]
}

emtfshower = {
    "int" : [
        "bx",
        "region",
        "sector",
        "isTwoLooseInTime",
        "isOneNominalInTime",
        "isTwoLooseOutOfTime",
        "isOneNominalOutOfTime",
    ]
}

l1mu = {
    "float" : ["pt","eta","phi"],
    "int" : ["charge","bx","tpid"]
}

l1mushower = {
    "int" : [
        "bits",
        "bx",
        "isTwoLooseInTime",
        "isOneNominalInTime",
        "isTwoLooseOutOfTime",
        "isOneNominalOutOfTime",
    ]
}

l1track = {
    "float" : ["pt","eta","phi"],
    "int" : ["charge","bx","tpid"]
}

l1trackmuon = {
    "float" : ["pt","eta","phi"],
    "int" : ["charge","bx","tpid"]
}

class Struct():
    def __init__(self, struct):
        self.name = struct
        self.objects = []

    def add(self, obj):
        self.objects.append(obj)

class Object():
    def __init__(self, prefix, obj_array):
        self.prefix = prefix
        self.obj_array = obj_array

eventStruct = Struct("EventStruct")
eventStruct.add(Object("event", event))

genGenParticleStruct = Struct("GenParticleStruct")
genGenParticleStruct.add(Object("gen", gen))

simTrackStruct = Struct("SimTrackStruct")
simTrackStruct.add(Object("sim", sim))

gemSimHitStruct = Struct("GEMSimHitStruct")
gemSimHitStruct.add(Object("gem_sh", gemsimhit))

cscSimHitStruct = Struct("CSCSimHitStruct")
cscSimHitStruct.add(Object("csc_sh", cscsimhit))

gemDigiStruct = Struct("GEMDigiStruct")
gemDigiStruct.add(Object("gem_digi", gemstrip))

cscDigiStruct = Struct("CSCDigiStruct")
cscDigiStruct.add(Object("csc_strip", cscstrip))
cscDigiStruct.add(Object("csc_comp", csccomp))
cscDigiStruct.add(Object("csc_wire", cscwire))

gemStubStruct = Struct("GEMStubStruct")
gemStubStruct.add(Object("gem_pad", gempad))
gemStubStruct.add(Object("gem_copad", gemcopad))
gemStubStruct.add(Object("gem_cluster", gemcluster))

cscStubStruct = Struct("CSCStubStruct")
cscStubStruct.add(Object("csc_alct", alct))
cscStubStruct.add(Object("csc_preclct", preclct))
cscStubStruct.add(Object("csc_clct", clct))
cscStubStruct.add(Object("csc_lct",  lct))
cscStubStruct.add(Object("csc_mplct", mplct))
cscStubStruct.add(Object("csc_shower",  cscshower))

l1MuStruct = Struct("L1MuStruct")
l1MuStruct.add(Object("emtftrack", emtftrack))
l1MuStruct.add(Object("emtfcand", emtfcand))
l1MuStruct.add(Object("emtfshower", emtfshower))
l1MuStruct.add(Object("l1mu", l1mu))
l1MuStruct.add(Object("l1mushower", l1mushower))

l1TrackStruct = Struct("L1TrackStruct")
l1TrackStruct.add(Object("l1track", l1track))
l1TrackStruct.add(Object("l1trackmuon", l1trackmuon))

structs = [
    eventStruct,
    genGenParticleStruct,
    simTrackStruct,
    gemSimHitStruct,
    cscSimHitStruct,
    gemDigiStruct,
    cscDigiStruct,
    gemStubStruct,
    cscStubStruct,
    l1MuStruct,
    l1TrackStruct
]


ffile = open("FlatStruct.h",'w')

# header
ffile.write("#ifndef GEMCode_GEMValidation_FlatStruct\n")
ffile.write("#define GEMCode_GEMValidation_FlatStruct\n\n")
ffile.write('#include "TTree.h"\n')
ffile.write('#include <vector>\n')
ffile.write('#include <string>\n\n')
ffile.write('typedef std::vector<float> t_floats;\n')
ffile.write('typedef t_floats* p_floats;\n')
ffile.write('typedef std::vector<int> t_ints;\n')
ffile.write('typedef t_ints* p_ints;\n')
ffile.write('typedef std::vector<t_ints> t_vints;\n')
ffile.write('typedef t_vints* p_vints;\n\n')

ffile.write("namespace my {\n")

def printStruct(ffile, struct):
    ffile.write("\n  struct %s {\n"%struct.name)

def printEndStruct(ffile):
    ffile.write("  };\n")

def printDeclaration(ffile, struct):
    ffile.write("\n")
    for obj in struct.objects:
        for stype in obj.obj_array:
            for var in obj.obj_array[stype]:
                ffile.write("    p_%ss %s_%s;\n"%(stype, obj.prefix, var))

def printInit(ffile, struct):
    ffile.write("\n    void init() {\n")
    for obj in struct.objects:
        for stype in obj.obj_array:
            for var in obj.obj_array[stype]:
                ffile.write("      %s_%s = new t_%ss;\n"%(obj.prefix, var, stype))
    ffile.write("    }\n")

def printBook(ffile, struct):
    ffile.write("\n    void book(TTree* t) {\n")
    for obj in struct.objects:
        for stype in obj.obj_array:
            for var in obj.obj_array[stype]:
                ffile.write('      t->Branch("%s_%s", &%s_%s);\n'%(obj.prefix, var, obj.prefix, var))
    ffile.write("    }\n")

for struct in structs:
    printStruct(ffile, struct)
    printDeclaration(ffile, struct)
    printInit(ffile, struct)
    printBook(ffile, struct)
    printEndStruct(ffile)

ffile.write("};\n")

ffile.write("\n#endif\n")

ffile.close()
