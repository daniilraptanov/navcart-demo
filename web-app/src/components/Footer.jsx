import { useAppWebSocket } from "../hooks/useWebSocket";

export const Footer = () => {
    const { statusText, isConnected } = useAppWebSocket();

    return (
        <footer className="bg-white border-t border-slate-200 px-6 py-3 flex justify-between items-center text-sm z-10">
            <span className="text-slate-500 font-medium">Cart connection:</span>
            <div className="flex items-center gap-2">
                <span className={`font-medium ${isConnected ? 'text-green-600' : 'text-amber-500'}`}>
                    {statusText}
                </span>
            </div>
        </footer>
    );
}
