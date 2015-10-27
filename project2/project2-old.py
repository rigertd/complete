def changeslow(arr, val):
    for i, x in reversed(list(enumerate(arr))):
        if x == val:
            coin_arr = [0] * len(arr)
            coin_arr[i] += 1
            coin_count = 1
            return coin_arr, coin_count

    temp_arr = []
    coin_arr = []
    temp_count = 0
    coin_count = 0
    for i, x in reversed(list(enumerate(arr))):
        if x < val:
            temp_arr, temp_count = changeslow(arr, val - x)
            temp_count += 1
            temp_arr[i] += 1
            if coin_count == 0 or temp_count <= coin_count:
                coin_count = temp_count
                coin_arr = temp_arr
    return coin_arr, coin_count

def changedp(arr, val):
    if arr[0] != 1:
        raise ValueError("The first element of arr must be 1.")
    lookup_table = [([0] * len(arr), 0)]
    for i in range(1, val + 1):
        min_count = None
        coin_idx = None
        for j, v in enumerate(arr):
            if v <= i and (min_count == None or lookup_table[i - v][1] + 1 < min_count):
                min_count = lookup_table[i - v][1] + 1
                coin_idx = j

        # make a copy of counts for current val - coin val
        count_arr = lookup_table[i - arr[coin_idx]][0][:]
        count_arr[coin_idx] += 1
        lookup_table.append((count_arr, min_count))
    
    return lookup_table[val]
