import NavBar from "./components/navbar";
import Infos from "./components/infos";

import { Route } from 'react-router-dom';

import routeMdw from './middleware/route';

import HomePage from './pages/home';

const routes = [
  { path: routeMdw.urlIndex(), name: 'homepage', Component: HomePage },
]

function App() {
  return <div>
    <NavBar />
    <Infos />
    <div className="page">
      {routes.map(({ path, Component }) => (
        <Route path={path} key={path} exact component={Component} />
      ))}
    </div>
  </div>;
}

export default App;
