import React, { useState } from 'react';

export default function App() {
  const [mode, setMode]     = useState('neighbors');
  const [inputA, setInputA] = useState('');
  const [inputB, setInputB] = useState('');
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const modes = [
    { key: 'neighbors', label: 'Neighbors', inputs: ['Node'] },
    { key: 'bfs',       label: 'BFS Order', inputs: ['Start Node'] },
    { key: 'shortest',  label: 'Shortest Path', inputs: ['Start Node', 'End Node'] },
    { key: 'dijkstra',  label: 'Dijkstra', inputs: ['Start Node', 'End Node'] },
  ];

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setResult(null);
    try {
      let url = `http://localhost:8080/${mode}`;
      if (mode === 'neighbors') url += `?node=${inputA}`;
      if (mode === 'bfs')       url += `?start=${inputA}`;
      if (mode === 'shortest' || mode === 'dijkstra') {
        url += `?start=${inputA}&end=${inputB}`;
      }
      const res = await fetch(url);
      const data = await res.json();
      if (!res.ok) throw new Error(data.error || 'Unknown error');
      setResult(data);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const cur = modes.find(m => m.key === mode);

  return (
    <div className="container py-5">
      <div className="card mx-auto" style={{ maxWidth: '600px' }}>
        <div className="card-body">
          <h1 className="card-title mb-4">GraphNexus Explorer</h1>

          <div className="btn-group mb-4 w-100">
            {modes.map(m => (
              <button
                key={m.key}
                onClick={() => { setMode(m.key); setResult(null); setError(null); }}
                className={`btn ${mode===m.key ? 'btn-primary' : 'btn-outline-secondary'}`}
                style={{ flex: 1 }}
              >
                {m.label}
              </button>
            ))}
          </div>

          <form onSubmit={handleSubmit}>
            <div className="row g-3 mb-3">
              <div className="col">
                <label className="form-label">{cur.inputs[0]}</label>
                <input
                  type="number"
                  className="form-control"
                  required
                  value={inputA}
                  onChange={e => setInputA(e.target.value)}
                />
              </div>
              {cur.inputs[1] && (
                <div className="col">
                  <label className="form-label">{cur.inputs[1]}</label>
                  <input
                    type="number"
                    className="form-control"
                    required
                    value={inputB}
                    onChange={e => setInputB(e.target.value)}
                  />
                </div>
              )}
            </div>
            <button type="submit" className="btn btn-success" disabled={loading}>
              {loading ? 'Running…' : 'Run'}
            </button>
          </form>

          {error && (
            <div className="alert alert-danger mt-4">
              Error: {error}
            </div>
          )}
          {result && (
            <pre className="mt-4 bg-light p-3 border rounded">
              {JSON.stringify(result, null, 2)}
            </pre>
          )}
        </div>
      </div>
    </div>
  );
}
