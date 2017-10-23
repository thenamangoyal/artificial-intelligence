import sys
import copy
import random

N = 0
parents = {}
orginialCPTs = [] # One entry of the form Variables, Dict((True, False)=>Probability)

def join_vars(acpt, bcpt):
    """
    acpt: (Variables, Dict((True, False)=>Probability))
    bcpt: (Variables, Dict((True, False)=>Probability))
    """
    join_cpt = []
    common = [i for i in acpt[0] if i in bcpt[0]]
    join_cpt.append([i for i in acpt[0] if i not in common] + common + [i for i in bcpt[0] if i not in common])
    join_cpt.append({})
    indexa = [acpt[0].index(c) for c in common]
    indexb = [bcpt[0].index(c) for c in common]

    for akey, aval in acpt[1].iteritems():
        a_key_var = list(akey)
        common_key_a = [a_key_var[i] for i in indexa]
        a_key_var = [v for i,v in enumerate(a_key_var) if i not in indexa]
        for bkey, bval in bcpt[1].iteritems():
            b_key_var = list(bkey)
            common_key_b = [b_key_var[i] for i in indexb]
            b_key_var = [v for i,v in enumerate(b_key_var) if i not in indexb]
            if common_key_a == common_key_b:
                join_cpt[1][tuple(a_key_var+common_key_a+b_key_var)] = aval*bval
    return join_cpt

def reduce_from_evidence(cpt, evidence):
    """
    cpt: (Variables, Dict((True, False)=>Probability))
    evidence: dictionary of variable = True/ False
    """
    reduce_cpt = copy.deepcopy(cpt)
    for var in evidence.keys():
        if var in reduce_cpt[0]:
            pos = (reduce_cpt[0]).index(var)
            reduce_cpt[1] = {k:v for k,v in reduce_cpt[1].iteritems() if k[pos] == evidence[var]}

    return reduce_cpt

def sum_var(cpt, var):
    """
    cpt: (Variables, Dict((True, False)=>Probability))
    var: Variable
    """

    sum_cpt = copy.deepcopy(cpt)
    if var in sum_cpt[0]:
        pos = sum_cpt[0].index(var)
        sum_cpt[0].remove(var)
        oldProbTable = sum_cpt[1]
        sum_cpt[1] = {}
        for key, value in oldProbTable.iteritems():
            new_key = list(key)
            new_key.pop(pos)
            new_key = tuple(new_key)
            if new_key in sum_cpt[1]:
                (sum_cpt[1])[new_key] += value
            else:
                (sum_cpt[1])[new_key] = value
    return sum_cpt

def normalize_vars(cpt,vars):
    """
    cpt: (Variables, Dict((True, False)=>Probability))
    vars: List of variables
    """
    normalize_cpt = []
    normalize_cpt.append(vars + [i for i in cpt[0] if i not in vars])
    normalize_cpt.append({})
    sum_normalize = {}
    index_vars = []
    for x in vars:
        index_vars.append(cpt[0].index(x))
    for key, val in cpt[1].iteritems():
        key_var = list(key)

        new_key_var = [key_var[i] for i in index_vars]
        rest_key_var = [v for i,v in enumerate(key_var) if i not in index_vars]

        new_key = tuple(rest_key_var)
        if new_key in sum_normalize:
            sum_normalize[new_key] += val
        else:
            sum_normalize[new_key] = val

    for key, val in cpt[1].iteritems():
        key_var = list(key)

        new_key_var = [key_var[i] for i in index_vars]
        rest_key_var = [v for i,v in enumerate(key_var) if i not in index_vars]

        new_key = tuple(new_key_var+rest_key_var)
        if sum_normalize[tuple(rest_key_var)] != 0:
            normalize_cpt[1][new_key] = val/sum_normalize[tuple(rest_key_var)]
        else:
            normalize_cpt[1][new_key] = 0

    return normalize_cpt


def variable_eliminiation(query_vars, query_given_vars):
    """
    query_vars: dictionary of variable = True/ False
    query_given_vars: dictionary of variable = True/ False
    """
    hidden_vars = [x for x in range(1,N+1) if (x not in query_vars.keys()) and ((x not in query_given_vars.keys()))]
    ve_cpts = [reduce_from_evidence(x, query_given_vars) for x in orginialCPTs]

    while len(hidden_vars)>0:
        h = hidden_vars.pop()
        index_with_h = [i for i,v in enumerate(ve_cpts) if h in v[0]]
        while len(index_with_h) > 1:
            index_h1 = index_with_h[0]
            index_h2 = index_with_h[1]

            joined_cpt = join_vars(ve_cpts[index_h1], ve_cpts[index_h2])
            ve_cpts = [v for i,v in enumerate(ve_cpts) if (i != index_h1 and i!=index_h2)]
            ve_cpts.append(joined_cpt)
            index_with_h = [i for i,v in enumerate(ve_cpts) if h in v[0]]
        if len(index_with_h) == 1:
            index_h = index_with_h[0]

            ve_cpts[index_h] = sum_var(ve_cpts[index_h], h)

    while len(ve_cpts) > 1:
        acpt = ve_cpts.pop()
        bcpt = ve_cpts.pop()
        joined_cpt = join_vars(acpt, bcpt)
        ve_cpts.append(joined_cpt)

    if len(ve_cpts) == 1:
        ve_cpts[0] = normalize_vars(ve_cpts[0], query_vars.keys())
        query = []
        for x in ve_cpts[0][0]:
            if x in query_vars:
                query.append(query_vars[x])
            elif x in query_given_vars:
                query.append(query_given_vars[x])
        return ve_cpts[0][1][tuple(query)]

    return 0

def random_sampling(query_vars, query_given_vars, no_sample):
    """
    query_vars: dictionary of variable = True/ False
    query_given_vars: dictionary of variable = True/ False
    """
    serial_order = get_serializable_order()
    sample_dist = {}

    for n_samp in range(0, no_sample):
        consistent = True       
        sample = {}
        for s in serial_order:
            selected_cpt = next((cpt[1] for cpt in orginialCPTs if (cpt[0] == [s] + parents[s])),None)
            if selected_cpt != None:
                query = [sample[p] for p in parents[s]]
                true_prob = selected_cpt[tuple([True] + query)]
                x = random.random()
                if x < true_prob:
                    if s in query_given_vars and query_given_vars[s] == False:
                        consistent = False
                        break
                    else:
                        sample[s] = True
                else:
                    if s in query_given_vars and query_given_vars[s] == True:
                        consistent = False
                        break
                    else:
                        sample[s] = False
        if consistent:
            key_dist = tuple([sample[k] for k in query_vars.keys()])
            if key_dist in sample_dist:
                sample_dist[key_dist] += 1
            else:
                sample_dist[key_dist] = 1
    
    total_no_samples = sum(sample_dist.values())
    query_key = tuple(query_vars.values())
    if total_no_samples != 0 and query_key in sample_dist:
        return float(sample_dist[query_key])/float(total_no_samples)
    return 0

def get_serializable_order():

    serialize_order = []
    variables = set(range(1, N+1))
    while len(variables) > 0:
        var_add = []
        for var in variables:
            found = True
            for p in parents[var]:
                if p in (variables - set([var])):
                    found = False
                    break
            if found:
                var_add.append(var)
        for x in var_add:
            serialize_order.append(x)
            variables.remove(x)
    return serialize_order

def print_cpt(cpt):
    return

# Main Function
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print "Please provide input bayes.txt and query.txt"
        sys.exit()
    try:
        fp = open(sys.argv[1],"r")
    except IOError:
        print "Could not read file:", sys.argv[1]
        sys.exit()

    N = int(fp.readline().strip())
    for i in range(0,N):
        line = fp.readline().strip().split()
        variables = [int(w) for w in line]
        x = variables[0]
        parent_list = variables[1:]
        parents[x] = parent_list
        probTable = {}
        for j in range(0,2**len(parent_list)):
            values = [float(w) for w in fp.readline().strip().split()]
            swap_list = [((j>>k)%2 == 0) for k in reversed(range(0,len(parent_list)))]
            probTable[tuple([True] + (swap_list))] = values[0]
            probTable[tuple([False] + (swap_list))] = values[1]
        orginialCPTs.append([variables,probTable])

    fp.close()

    try:
        fq = open(sys.argv[2],"r")
    except IOError:
        print "Could not read file:", sys.argv[2]
        sys.exit()

    given_no_sample = 100000
    if len(sys.argv) >= 4:
        given_no_sample = int(sys.argv[3])
        if given_no_sample < 0:
            print "No of samples for rejection sampling should be a non negative number."
            sys.exit()

    for line in fq.readlines():
        split_line = line.strip().split()
        if len(split_line) >= 1 and split_line[0] == "ve":
            if len(split_line) >= 2 and split_line[1] == "q":
                n = 2
                collecting_evidence = False
                valid = True
                variables = {}
                evidence = {}

                while n < len(split_line):
                    if split_line[n] == "e":
                        collecting_evidence = True
                    elif split_line[n].find('~') != -1:
                        pos = split_line[n].find('~')
                        var = int(split_line[n][pos+1:])
                        if var <= 0 or var > N:
                            valid = False
                            print "Invalid query"
                            break
                        if collecting_evidence == False:
                            variables[var] = False
                        else:
                            evidence[var] = False
                    else:
                        var = int(split_line[n])
                        if var <= 0 or var > N:
                            valid = False
                            print "Invalid query"
                            break
                        if collecting_evidence == False:
                            variables[var] = True
                        else:
                            evidence[var] = True
                    n+=1
                if valid:
                    print variable_eliminiation(variables, evidence)

        elif len(split_line) >= 1 and split_line[0] == "rs":
            if len(split_line) >= 2 and split_line[1] == "q":
                n = 2
                collecting_evidence = False
                valid = True
                variables = {}
                evidence = {}

                while n < len(split_line):
                    if split_line[n] == "e":
                        collecting_evidence = True
                    elif split_line[n].find('~') != -1:
                        pos = split_line[n].find('~')
                        var = int(split_line[n][pos+1:])
                        if var <= 0 or var > N:
                            valid = False
                            print "Invalid query"
                            break
                        if collecting_evidence == False:
                            variables[var] = False
                        else:
                            evidence[var] = False
                    else:
                        var = int(split_line[n])
                        if var <= 0 or var > N:
                            valid = False
                            print "Invalid query"
                            break
                        if collecting_evidence == False:
                            variables[var] = True
                        else:
                            evidence[var] = True
                    n+=1
                if valid:
                    print random_sampling(variables, evidence, given_no_sample)
    fq.close()