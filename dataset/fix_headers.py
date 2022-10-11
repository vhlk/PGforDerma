import pandas as pd

data = pd.read_csv("Dermatology.csv")

num_lines = data.last_valid_index() + 1
for column in data.columns:
    data.rename(columns={column : f'{column}:{num_lines}:<int>'}, inplace=True)

data.to_csv('Dermatology_new.csv', index_label=f"INDEX:{num_lines}:<int>")