
# functions operate on awkward arrays
def has_gem_pad(my_tree):
    return has_object(my_tree, "sim_id_gem_pad")

def has_gem_copad(my_tree):
    return has_object(my_tree, "sim_id_gem_copad")

def has_csc_alct(my_tree):
    return has_object(my_tree, "sim_id_csc_alct")

def has_csc_clct(my_tree):
    return has_object(my_tree, "sim_id_csc_clct")

def has_csc_lct(my_tree):
    return has_object(my_tree, "sim_id_csc_lct")

def has_csc_mplct(my_tree):
    return has_object(my_tree, "sim_id_csc_mplct")

def has_emtf_track(my_tree):
    return has_object(my_tree, "sim_id_emtf_track")

def has_emtf_cand(my_tree):
    return has_object(my_tree, "sim_id_emtf_cand")

def has_l1mu(my_tree):
    return has_object(my_tree, "sim_id_l1mu")

def has_object(my_tree, objstring):
    obj_array = my_tree.array(objstring)
    obj_array = ak.flatten(obj_array, axis=1)
    has_obj = ak.num(obj_array) > 0
    return has_obj


## convert 1D array to histogram
def efficiency(denominator, num_cut, bins):
    numerator = denominator * num_cut
    th1_denom = TH1F("denom","denom", bins, np.array(denominator))
    th1_num = TH1F("num","num", bins, np.array(numerator))
