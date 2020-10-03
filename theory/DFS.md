# DFS
    def dfs(v):

        mark[v] = True
        for u in neighbors(v):
            if not mark[v]:
                dfs(u)

# Color adj components:
    component_number = 0

    def dfs(v):
        mark[v] = True
        for u in neighbors(v):
            if not mark[v]:
                dfs(u)
        component[v] = component_number
    
    for v in all_vertexes:
        if not mark[v]:
            dfs(v)
            component_number += 1 // It`s the index of the component at the moment, but in the end it's equal to the total number of components


# The time of entrance and exit (time is a metaphor)

    def dfs(v):
        times_in[v] = time
        time += 1        
    
        mark[v] = True
        for u in neighbors(v):
            if not mark[v]:
                dfs(u)

        times_in[v] = time
        time += 1        
