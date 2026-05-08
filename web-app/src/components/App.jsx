import { Header } from './Header';
import { Main } from './main/Main';
import { Footer } from './Footer';

export const App = () => {
    return (
        <div className="flex flex-col h-screen overflow-hidden bg-slate-50 text-slate-800 font-sans">            
            <Header />
            <Main />
            <Footer />
        </div>
    );
}
