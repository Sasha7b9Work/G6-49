#'''
def Frequency(N):
      return 1e8 / (1 << 32) * N;

for n in range((1 << 32) - 30, (1 << 32) - 1):
      print (n),
      print (Frequency(n)),
      print ("delta F = ", Frequency(n) - Frequency(n-1))
#'''

'''
def Register(f):
      return f / 1e8 * (1 << 32)

for f in range(990, 1010):
      print f,
      print Register(f),
      print Register(f) - Register(f - 1)
'''
