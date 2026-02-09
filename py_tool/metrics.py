from sklearn.metrics import classification_report

targets = [ 1, 2, 1, 0, 3, 0, 3, 3, 1, 5, 6, 1, 3, 0, 0, 5, 1, 6, 1, 6, 3, 0, 3, 1, 0, 0, 4, 0, 6, 0, 3, ]
outputs = [ 1, 2, 1, 0, 3, 0, 3, 3, 1, 5, 6, 1, 3, 0, 0, 5, 1, 6, 1, 6, 3, 0, 3, 1, 0, 0, 4, 0, 6, 0, 3, ]

print(classification_report(targets, outputs, digits=5))
