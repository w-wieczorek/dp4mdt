from sklearn.metrics import classification_report

targets = [ 0, 1, 0, 0, 0, 1, 0, 0, 4, 1, 0, 1, 2, 0, 0, 6, 0, 1, 0, 0, 2, 0, 0, 0, 0, 6, 3, 5, 0, 2 ]
outputs = [ 0, 1, 0, 0, 0, 1, 0, 0, 4, 1, 0, 1, 0, 0, 1, 6, 0, 1, 0, 0, 2, 0, 0, 0, 0, 6, 3, 5, 0, 1 ]

print(classification_report(targets, outputs, digits=5))
