"""
The purpose of this programming project is to explore resource allocation algorithms. 
This will be achieved by implementing the banker's algorithm.
"""

"""
 Joseph Mace
 Operating Systems
 Programming Project #4: Bankers Algorithm
 March 31, 2025
"""

class BankersAlgorithm:
    def __init__(self, available, max_demand, allocation):
        self.available = available
        self.max_demand = max_demand
        self.allocation = allocation
        self.num_processes = len(max_demand)
        self.num_resources = len(available)
        self.need = [[self.max_demand[i][j] - self.allocation[i][j]
                      for j in range(self.num_resources)]
                     for i in range(self.num_processes)]

    def is_safe_state(self):
        """Check if system is in a safe state"""
        work = self.available[:]
        finish = [False] * self.num_processes
        safe_sequence = []

        while True:
            found = False
            for i in range(self.num_processes):
                if not finish[i] and all(self.need[i][j] <= work[j] for j in range(self.num_resources)):
                    work = [work[j] + self.allocation[i][j] for j in range(self.num_resources)]
                    finish[i] = True
                    safe_sequence.append(i)
                    found = True
                    break
            
            if not found:
                break

        return all(finish), safe_sequence

    def request_resource(self, process_id, request):
        """Handle resource request for a given process"""
        # Check if the request exceeds the maximum need for the process
        if any(request[j] > self.need[process_id][j] for j in range(self.num_resources)):
            print(f"Request exceeds the maximum need for process {process_id}.")
            return False
    
        # Check if there are enough available resources
        if any(request[j] > self.available[j] for j in range(self.num_resources)):
            print(f"Not enough resources available for process {process_id}.")
            return False
    
        # Save the original state for rollback
        original_available = self.available[:]
        original_allocation = self.allocation[process_id][:]  # Copy the current allocation
        original_need = self.need[process_id][:]
    
        # Temporarily apply the request (to a copy)
        temp_allocation = [row[:] for row in self.allocation]
        temp_need = [row[:] for row in self.need]
        temp_available = self.available[:]  # Start with original available resources
    
        # Apply the request: subtract request from available, add to allocation and need
        temp_available = [temp_available[j] - request[j] for j in range(self.num_resources)]
        temp_allocation[process_id] = [temp_allocation[process_id][j] + request[j] for j in range(self.num_resources)]
        temp_need[process_id] = [temp_need[process_id][j] - request[j] for j in range(self.num_resources)]
    
        # Check if the system is in a safe state after the request
        banker_temp = BankersAlgorithm(temp_available, self.max_demand, temp_allocation)
        safe, _ = banker_temp.is_safe_state()
        
        if not safe:
            # If the system is unsafe, rollback to the original state
            print(f"Request by process {process_id} would result in an unsafe state. Rolling back.")
            self.available = original_available
            self.allocation = original_allocation
            self.need = original_need
            return False
    
        # If the system is safe, the request can be granted
        self.available = temp_available
        self.allocation = temp_allocation
        self.need = temp_need
        print("\nTHE REQUEST CAN BE GRANTED!")
        return True

def get_matrix_input(n, m, name):
    """Function to take matrix input from user"""
    print(f"Enter the {name} matrix ({n} x {m}):")
    matrix = []
    for i in range(n):
        row = list(map(int, input().split()))
        while len(row) != m:
            print(f"Invalid input! Please enter {m} integers for row {i + 1}.")
            row = list(map(int, input().split()))
        matrix.append(row)
    return matrix


def get_vector_input(m, name):
    """Function to take vector input from user"""
    print(f"Enter the {name} vector ({m} elements):")
    vector = list(map(int, input().split()))
    while len(vector) != m:
        print(f"Invalid input! Please enter {m} integers.")
        vector = list(map(int, input().split()))
    return vector

# ---- Main Program ----
# 1. Get the number of processes and resources
n = int(input("Enter the number of processes: "))
m = int(input("Enter the number of resource types: "))

# 2. Get the allocation matrix
allocation = get_matrix_input(n, m, "allocation")

# 3. Get the max matrix
max_demand = get_matrix_input(n, m, "max demand")

# 4. Get the available vector
available = get_vector_input(m, "available resources")

# 5. Create BankersAlgorithm instance
banker = BankersAlgorithm(available, max_demand, allocation)

# 6. Check initial safety
safe, sequence = banker.is_safe_state()
if safe:
    print("The system is in a safe state.")
    print("Safe sequence:", sequence)
else:
    print("The system is in an unsafe state!")
    
# 7 Echo processes#, recource#, allocation matrix, max matrix
print(f"\nThere are {n} processes in the system.")
print(f"\nThere are {m} resource types.")
print("\nAllocation Matrix:")
for row in allocation:
    print(" ".join(map(str, row)))

print("\nMax Demand Matrix:")
for row in max_demand:
    print(" ".join(map(str, row)))
    
# Compute and output the need matrix
need = [[max_demand[i][j] - allocation[i][j] for j in range(m)] for i in range(n)]
print("\nNeed Matrix:")
for row in need:
    print(" ".join(map(str, row)))

# Echo Available
print("\nThe available Vector is...")
print(available)

# 9. Process a resource request
while True:
    choice = input("\nDo you want to request resources? (yes/no): ").strip().lower()
    if choice == 'no':
        break

    request_input = input("Enter request in format 'i:xxxx' (e.g., 1:0420 for process 1 requesting [0, 4, 2, 0]): ").strip()

    # Split process ID and request vector
    if ':' not in request_input:
        print("Invalid format! Use 'i:xxxx' format.")
        continue
    
    process_id_str, request_str = request_input.split(':', 1)

    # Validate process ID
    if not process_id_str.isdigit():
        print("Invalid process ID! Must be an integer.")
        continue

    process_id = int(process_id_str)
    if process_id < 0 or process_id >= n:
        print(f"Invalid process ID! Must be between 0 and {n-1}.")
        continue

    # Convert request vector string (e.g., "0420") into a list of integers
    if not request_str.isdigit() or len(request_str) != m:
        print(f"Invalid request vector! Must be exactly {m} digits (one per resource type).")
        continue

    request = [int(c) for c in request_str]  # Convert "0420" -> [0, 4, 2, 0]

    # Process the request
    granted = banker.request_resource(process_id, request)

    # Assuming 'available' and 'request' are already defined
    if granted:
        safe, sequence = banker.is_safe_state()
        
        if safe:
            # Update the available vector after granting the request
            for i in range(len(request)):
                available[i] -= request[i]
            
            # Print the final available vector (which is the updated one)
            print("\nAvailable Vector is...")
            print(" ".join(map(str, available)))  # Use 'available' instead of 'new_available'
        else:
            print("After allocation, the system has become unsafe!")
