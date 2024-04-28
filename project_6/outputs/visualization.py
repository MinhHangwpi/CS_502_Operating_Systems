import pandas as pd
import matplotlib.pyplot as plt

# Function to create and save plots
def create_and_save_plot(csv_file, plot_title, group_by, output_filename):
    # Load data from CSV file
    df = pd.read_csv(csv_file)

    # Metrics to plot
    metrics = ["cpu_time", "wall_clock_time", "involuntary_context_switches", "voluntary_context_switches",
               "major_page_faults", "minor_page_faults", "max_rss"]

    # Setting up the plot
    fig, axes = plt.subplots(nrows=3, ncols=4, figsize=(20, 15), gridspec_kw={'hspace': 1, 'wspace': 1})
    fig.suptitle(plot_title)

    # Flatten the axes array for easier iteration
    axes = axes.flatten()

    # Plot each metric
    for i, metric in enumerate(metrics):
        # Create a bar chart for each metric grouped by read_size
        df_grouped = df.groupby(group_by)[metric].mean()  # Calculating mean for each group
        ax = df_grouped.plot(kind='bar', ax=axes[i], color='skyblue')
        ax.set_title(metric.replace('_', ' ').title())
        ax.set_xlabel(group_by)
        ax.set_ylabel('Value')
        # Rotate x labels to 45 degrees for better readability
        for label in ax.get_xticklabels():
            label.set_rotation(45)
            label.set_ha('right')  # Align the labels to the right for better legibility

    # Remove any unused subplots
    for i in range(len(metrics), len(axes)):
        fig.delaxes(axes[i])

    # Adjust layout and save to file
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig(output_filename)
    plt.close()

# Create and save plot for read_size grouped data
create_and_save_plot('./read_results.csv', 'Performance Metrics Grouped by Read Size', 'read_size', 'read_size_performance.png')

# Create and save plot for num_threads grouped data
create_and_save_plot('./mmap_results.csv', 'Performance Metrics Grouped by Number of Threads', 'num_threads', 'num_threads_performance.png')
