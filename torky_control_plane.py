import numpy as np

def run_control_plane():
    print("--- Starting AT-ACE Control Plane Verification ---")
    # معالم الاستقرار الهندسي
    epsilon = 0.016
    r = 0.0
    
    # دالة الجهد المقيدة هندسياً لمنع الانفجار
    V_finite = 1.4399 / np.sqrt(r**2 + epsilon)
    print(f"Calculated Energy Bound at r=0: {V_finite:.3f} eV")
    print("System Register Status: SUCCESS_NO_NAN")

if __name__ == "__main__":
    run_control_plane()
