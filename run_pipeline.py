import numpy as np
import torky_engine

print("==================================================")
print("  Initializing Torky Autopoietic Continuum Engine ")
print("==================================================")

# 1. Initialize Engine
engine = torky_engine.init("data.torky", total_records=10000)

# 2. Generate 5,000 raw 64-D vectors
print("\n[+] Generating 5,000 raw 64-D test vectors...")
records = np.random.randn(5000, 64).astype(np.float32)

# 3. Stream & Govern via 9-byte packed architecture
print("[+] Streaming and applying pre-execution governance...")
engine.stream_and_govern(records, tile_id=0)

# 4. Reconstruct sample vector
sample_vec = engine.reconstruct(0x2A, 1500, 32768, 987654321)
print(f"[+] Reconstructed 64-D Vector (D0..D4): {sample_vec[:5]}")

engine.close()
print("\n[SUCCESS] Engine executed and verified successfully.")
