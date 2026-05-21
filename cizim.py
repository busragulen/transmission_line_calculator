import pandas as pd
import matplotlib.pyplot as plt

# hat profili cizimi
try:
    df_hat = pd.read_csv('hat_profili.csv')
    
    fig, ax1 = plt.subplots(figsize=(10, 6))

    color = 'tab:red'
    ax1.set_xlabel('Hat Uzunluğu - x (km)', fontweight='bold')
    ax1.set_ylabel('Gerilim (kV)', color=color, fontweight='bold')
    ax1.plot(df_hat['x'], df_hat['V_FazFaz'], color=color, marker='o', label='Gerilim')
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.grid(True, linestyle='--', alpha=0.6)

    ax2 = ax1.twinx()  
    color_p = 'tab:blue'
    color_q = 'tab:green'
    ax2.set_ylabel('Güç (MW / MVAr)', color='black', fontweight='bold')
    ax2.plot(df_hat['x'], df_hat['P'], color=color_p, marker='s', label='Aktif Güç (P)')
    ax2.plot(df_hat['x'], df_hat['Q'], color=color_q, marker='^', label='Reaktif Güç (Q)')
    ax2.tick_params(axis='y', labelcolor='black')

    fig.suptitle('Hat Uzunluğuna Bağlı Gerilim ve Güç Değişimi', fontsize=14, fontweight='bold')
    fig.legend(loc='upper right', bbox_to_anchor=(0.9, 0.85))
    plt.tight_layout()
    plt.savefig('Hat_Profili_Grafigi.png', dpi=300)
    plt.show()

except FileNotFoundError:
    print("hat_profili.csv bulunamadı.")

# p-v egrisi cizimi
try:
    df_pv = pd.read_csv('pv_egrisi.csv')
    
    plt.figure(figsize=(8, 6))
    plt.plot(df_pv['P1'], df_pv['U1'], marker='o', color='purple', linewidth=2)
    plt.title('P-V Eğrisi (Yük Değişimine Göre)', fontsize=14, fontweight='bold')
    plt.xlabel('Hat Başı Aktif Gücü - P1 (MW)', fontweight='bold')
    plt.ylabel('Hat Başı Gerilimi - U1 (kV)', fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig('PV_Egrisi_Grafigi.png', dpi=300)
    plt.show()

except FileNotFoundError:
    print("pv_egrisi.csv bulunamadı.")
