import matplotlib.pyplot as plt

# Data
categories = ["Identical\n+ List", "Identical\n+ RealTime", "Related\n+ List", "Related\n+ RealTime", "Unrelated\n+ List", "Unrelated\n+ RealTime"]
mean_values = [1.087, 1.107, 1.145, 1.299, 1.165, 1.287]

# Plotting
plt.figure(figsize=(10, 6))  # Set the figure size
bars = plt.bar(categories, mean_values, color=['orange'])

# Add labels and title
plt.xlabel("Category")
plt.ylabel("Mean Value")
plt.title("Mean Values for Different Categories")
plt.xticks(ha='center')  # Center-align the x-axis labels
# Add value labels on top of each bar
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width() / 2, height, f'{height:.3f}', ha='center', va='bottom')

# Show the plot
plt.tight_layout()
plt.savefig("../assets/images/case0_mean.svg", format="svg")