def function(x, y, ratio_x, ratio_y):
  x = x / ratio_x
  y = y / ratio_y
  output = ((x/7)**3 + (x/50)**2) / 32 - 8
  return output