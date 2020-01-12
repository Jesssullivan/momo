# implements an example potentiometer split function with variable limits.
# @ https://github.com/jesssullivan


def eval_range(val, rmax=1024, rmin=0):

    val = (val - rmin) * (1024 / rmax)

    if val / 4 >= 127:
        val = (val / 4) - 127
        if val*2 >= 254:
            return 'Throttle', 254
        else:
            return 'Throttle', int(val*2)
    else:
        val = (val / 4) - 127
        return 'Brake', -val*2


# default values:
for val in [0, 507, 512, 1024]:
    print(eval_range(val, rmax=1024, rmin=0))

# example reads, constrained:
for val in [20, 30, 40, 50, 60, 70 ,80 ,90]:
    print(eval_range(val, rmax=100, rmin=20))


