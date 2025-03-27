import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv("branch_prediction_results.csv")

# Exclude the last row if it's the average
if df.iloc[-1]['Trial'] == 'Average':
    avg_row = df.iloc[-1]
    df = df.iloc[:-1]
else:
    avg_row = df.mean(numeric_only=True)

# Convert columns to numeric (in case they are strings)
df[' Predictable (ns)'] = pd.to_numeric(df[' Predictable (ns)'])
df[' Unpredictable (ns)'] = pd.to_numeric(df[' Unpredictable (ns)'])
df[' Difference (ns)'] = pd.to_numeric(df[' Difference (ns)'])

# Plot predictable vs unpredictable loop timings
plt.figure(figsize=(10, 5))
plt.plot(df['Trial'], df[' Predictable (ns)'], label='Predictable Loop', marker='o')
plt.plot(df['Trial'], df[' Unpredictable (ns)'], label='Unpredictable Loop', marker='o')
plt.title("Loop Execution Time per Trial")
plt.xlabel("Trial")
plt.ylabel("Time (nanoseconds)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("loop_execution_time.png")
plt.show()

# Plot difference per trial
plt.figure(figsize=(10, 4))
plt.bar(df['Trial'], df[' Difference (ns)'], color='orange')
plt.title("Execution Time Difference per Trial")
plt.xlabel("Trial")
plt.ylabel("Difference (ns)")
plt.grid(True, axis='y')
plt.tight_layout()
plt.savefig("difference_per_trial.png")
plt.show()

# Print averages
print("=== Averages ===")
print(f"Predictable Loop (avg): {avg_row[' Predictable (ns)']:.2f} ns")
print(f"Unpredictable Loop (avg): {avg_row[' Unpredictable (ns)']:.2f} ns")
print(f"Difference (avg): {avg_row[' Difference (ns)']:.2f} ns")
