import random
import argparse
# the entirery of this file was written by ChatGPT
def generate_large_network(num_devices, avg_degree):
    """
    Generates a connected undirected graph with `num_devices` nodes and
    average degree `avg_degree`. Returns a string where the first line is
    num_devices, and each subsequent line is "u,v,speed".
    """
    num_edges = int(num_devices * avg_degree / 2)
    edges = set()

    # Build a spanning tree so the graph is connected
    nodes = list(range(num_devices))
    random.shuffle(nodes)
    for i in range(1, num_devices):
        u = nodes[i]
        v = nodes[random.randint(0, i - 1)]
        edges.add((min(u, v), max(u, v)))

    # Add random extra edges until we reach num_edges
    while len(edges) < num_edges:
        u = random.randint(0, num_devices - 1)
        v = random.randint(0, num_devices - 1)
        if u == v:
            continue
        edge = (min(u, v), max(u, v))
        if edge not in edges:
            edges.add(edge)

    # Speed categories: smaller number = faster link
    speed_values = [1, 2, 4, 8]       # 1=10Gbps, 2=1Gbps, 4=100Mbps, 8=10Mbps
    weights      = [0.1, 0.3, 0.4, 0.2]

    lines = [str(num_devices)]
    for u, v in sorted(edges):
        speed = random.choices(speed_values, weights)[0]
        lines.append(f"{u},{v},{speed}")

    return "\n".join(lines)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Generate a large network file with <num_devices> and average degree <avg_degree>."
    )
    parser.add_argument(
        "num_devices",
        type=int,
        help="Number of devices (vertices) in the network"
    )
    parser.add_argument(
        "avg_degree",
        type=float,
        help="Average degree (approx. links per node) in the network"
    )
    parser.add_argument(
        "-o", "--output",
        default=None,
        help="Output filename (if not provided, a name is auto-generated)"
    )

    args = parser.parse_args()

    # Generate the network text
    network_text = generate_large_network(args.num_devices, args.avg_degree)

    # Determine output filename
    if args.output:
        filename = args.output
    else:
        filename = f"devices_{args.num_devices}_avgdegree_{args.avg_degree}_large_network.txt"

    # Write to file
    with open(filename, "w") as f:
        f.write(network_text)

    print(f"Generated {filename} with {args.num_devices} devices and avg degree {args.avg_degree:.2f}.")
