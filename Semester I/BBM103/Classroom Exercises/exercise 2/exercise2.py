import sys
from operator import itemgetter

'''
pySet=['e','a','u','o','i']
pyDic={'e':1,'a':2,'u':3,'o':4,'i':5}
pyFSet=frozenset(('e','a','u','o','i'))

print(sorted(pySet, reverse=True))
print(sorted(pyDic, reverse=True))
print(sorted(pyFSet, reverse=True))

def takeSecond(elem):
    return elem[2]
random=[(2,2,'flower'),(3,4,'eye'),(4,1,'cow'),(1,3,'string')]
sortedList=sorted(random, key=takeSecond, reverse=True)
print('Sorted list: ', sortedList)
'''

'''
lis=[{'name':'Nandini','age':'20'},{'name':'Manjeet','age':'20'},{'name':'Nikhil','age':'19'},{'name':'Nikhil','age':'21'}]
print('The list printed sorting by age: ','\n',sorted(lis, key=itemgetter('age')))
print('The list printed sorting by age and name: ','\n',sorted(lis, key=itemgetter('age','name')))
print('The list printed sorting by name and age: ','\n',sorted(lis, key=itemgetter('name','age')))
print('The list printed sorting by age in descending order: ','\n',sorted(lis, key=itemgetter('age'),reverse=True))
'''

'''
x = {i:i**2 for i in range(10)}
print(x)

noprimes={j for i in range(2,8) for j in range(i*2,50,i)}
primes=[x for x in range(2,50) if x not in noprimes]
print('Noprimes: ',sorted(noprimes))
print('Primes: ', primes)
'''

non_flat=[[1,2,3],[4,5,6],[7,8]]
list=[y for x in non_flat for y in x]
print(list)
