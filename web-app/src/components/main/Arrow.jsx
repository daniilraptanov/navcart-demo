import { ArrowUp } from "../../svg/ArrowUp";

export const Arrow = () => {
    return (
        <div className="flex-1 w-full max-w-sm flex items-center justify-center bg-blue-50/50 rounded-[3rem] border border-blue-100 shadow-inner relative overflow-hidden">
            <div className="absolute inset-0 bg-blue-400 opacity-[0.03] animate-pulse"></div>            
            <ArrowUp />
        </div>
    );
}
