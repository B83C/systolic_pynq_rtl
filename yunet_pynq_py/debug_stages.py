"""Debug: check loaded stage table."""
from load_stages import load_stages
stages = load_stages('yunet_stages.npz')
print(f'Total stages: {len(stages)}')
for i, s in enumerate(stages[:5]):
    print(f'  [{i}] type={s["type"]!r} inp={s["inp"]!r} out={s["out"]!r}')
    if 'w' in s:
        print(f'       w={s["w"].shape} b={s["b_i32"].shape}')
print('...')
# Check if 'input' matching works
store = {'input': 'fake_img'}
for s in stages[:3]:
    inps = [store.get(n) for n in ([s['inp']] if isinstance(s['inp'], str) else s['inp'])]
    print(f'  stage {s["out"]}: inputs={inps}')
