import sys
from collections import defaultdict

def parse_log(file):
    calls = []
    for line in file:
        if "(" in line:
            call = line.split("(")[0].strip()
            calls.append(call)
    return calls

def count_calls(calls):
    counts = defaultdict(int)
    for call in calls:
        counts[call] += 1
    return counts

def count_sequence_pairs(calls):
    pairs = defaultdict(int)
    for i in range(len(calls)-1):
        pair = (calls[i], calls[i+1])
        pairs[pair] += 1
    return pairs

def main():
    calls = parse_log(sys.stdin)
    unique_calls = set(calls)
    print(f"AAA: {len(calls)} invoked system call instances from {len(unique_calls)} unique system calls")

    call_counts = count_calls(calls)

    for call, count in call_counts.items():
        print(f"{call} {count}")

    if len(sys.argv) > 1 and sys.argv[1] == "seq":
        seq_counts = count_sequence_pairs(calls)
        for (call1, call2), count in seq_counts.items():
            print(f"{call1}:{call2} {count}")

if __name__ == "__main__":
    main()
